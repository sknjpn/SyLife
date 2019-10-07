#include "Cell.h"

#include "PartModel.h"
#include "Egg.h"

#include "EggManager.h"
#include "ViewerManager.h"
#include "CellManager.h"
#include "SystemManager.h"

#include <boost/math/constants/constants.hpp>
using namespace boost::math::constants;

void CellModel::Draw(double a)
{
	// parts
	for (const auto& pc : m_partConfigs)
	{
		auto t2 = Transformer2D(Mat3x2::Rotate(pc->getRotation()).translated(pc->getPosition()));

		pc->getModel()->Draw(a);
	}
}

void CellModel::load_this(const ptree& pt)
{
	// parts
	for (auto part : pt.get_child("parts")) m_partConfigs.emplace_back(make_shared<PartConfig>())->load(part.second);

	UpdateProperties();

	Model::load_this(pt);
}

void CellModel::save_this(ptree& pt) const
{
	// parts
	{
		ptree parts;

		for (const auto& e : m_partConfigs)
		{
			ptree part; e->save(part);
			parts.push_back(std::make_pair("", part));
		}

		pt.add_child("parts", parts);
	}

	Model::save_this(pt);

	pt.put("type", "CellModel");
}

void CellModel::makeViewer()
{
	g_viewerManagerPtr->makeViewer<CellViewer>()->setModel(shared_from_this());
}


shared_ptr<PartConfig>& CellModel::AddPartConfig()
{
	return m_partConfigs.emplace_back(make_shared<PartConfig>());
}

void CellModel::UpdateProperties()
{
	// mass
	m_mass = accumulate(m_partConfigs.begin(), m_partConfigs.end(), 0.0, [](double mass, const auto& p) { return mass + p->getModel()->getMass(); });

	// center
	Vec2 center = accumulate(m_partConfigs.begin(), m_partConfigs.end(), Vec2::Zero(), [](Vec2 acc, const auto& p) { return acc + p->getModel()->getMass() * (p->getPosition() + p->getModel()->getCenter().rotated(p->getRotation())); }) / m_mass;

	// centerを原点に設定
	for (const auto& p : m_partConfigs) p->setPosition(p->getPosition() - center);

	// inertia
	m_inertia = accumulate(m_partConfigs.begin(), m_partConfigs.end(), 0.0, [](double acc, const auto& p) { return acc + p->getInertia(); });

	// radius
	m_radius = sqrt(2 * m_inertia / m_mass);

	// material
	m_material = accumulate(m_partConfigs.begin(), m_partConfigs.end(), Storage(), [](Storage acc, const auto& p) { return acc += p->getModel()->getMaterial(); });
}

CellState::CellState(const shared_ptr<CellModel>& model)
	: m_model(model)
	, m_startTimer(0.0)
	, m_deathTimer(25.0)
{
	SetMass(m_model->getMass());
	SetRadius(m_model->getRadius());
	SetInertia(m_model->getInertia());

	// parts
	for (const auto& pc : m_model->GetPartConfigs())
		m_partStates.emplace_back(pc->getModel()->makeState())->SetPartConfig(pc);
}

void CellState::UpdateCell()
{
	// 衝突処理
	{
		auto result = g_cellManagerPtr->GetCellStateKDTree().knnSearch(2, getPosition());
		if (result.size() == 2)
		{
			auto& t = g_cellManagerPtr->GetCellStates()[result[1]];

			if (t->getPosition() != getPosition() && (getRadius() + t->getRadius() - (t->getPosition() - getPosition()).length()) > 0)
			{
				auto f = -1000.0 * (t->getPosition() - getPosition()).normalized() * (getRadius() + t->getRadius() - (t->getPosition() - getPosition()).length());
				AddForceInWorld(f, getPosition());
				t->AddForceInWorld(-f, t->getPosition());
			}
		}
	}

	// Timer
	m_deathTimer -= g_systemManagerPtr->GetDeltaTime();
	m_startTimer += g_systemManagerPtr->GetDeltaTime();

	// parts
	for (const auto& p : m_partStates) p->Update(*this);

	// 接触したMoleculeStateの取り込み
	/*
	for (auto i : g_moleculeManagerPtr->GetMoleculeStateKDTree().knnSearch(1, getPosition()))
	{
		auto& m = g_moleculeManagerPtr->GetMoleculeStates()[i];

		if (!m->IsDestroyed() && (m->getPosition() - getPosition()).length() - getRadius() < 0.0) TakeMolecule(m);
	}
	*/

	// 余剰のMoleculeStateの投棄
	/*
	for (;;)
	{
		bool flag = true;

		for (const auto& m : m_storage.GetMolecules())
		{
			if (2 * m_model->getMaterial().Num(m.first) < m_storage.Num(m.first))
			{
				ExpireMolecule(m.first);

				flag = false;

				break;
			}
		}

		if (flag) break;
	}
	*/

	// 分裂処理
	if (m_storage >= m_model->getMaterial())
	{
		m_storage -= m_model->getMaterial();

		const auto& e = g_eggManagerPtr->AddEggState(m_model);
		e->setPosition(getPosition());
		e->setRotation(Random(boost::math::constants::pi<double>() * 2.0));
		e->SetVelocity(Vec2(1.0, 0.0).rotated(rand() / 360.0));
	}

	// 死亡処理
	if (m_deathTimer <= 0.0)
	{
		// MoleculeStateの吐き出し
		/*
		auto s = m_storage + m_model->getMaterial();
		for (const auto& m : s.GetMolecules())
		{
			for (unsigned int i = 0; i < m.second; i++)
			{
				// 吐き出す方向
				auto v = Vec2(1.0, 0.0).rotated(rand() / 3600.0);

				// 吐き出されたMoleculeState
				const auto& ms = g_moleculeManagerPtr->AddMoleculeState(m.first);
				ms->setPosition(getPosition() + v * (getRadius() + m.first->getRadius()) * Random(1.0));
				ms->SetVelocity(v * 0.1);
			}
		}
		*/

		Destroy();
	}
}

void CellState::Draw()
{
	auto t1 = Transformer2D(Mat3x2::Rotate(getRotation()).translated(Vec2(getPosition().x, getPosition().y)));
	auto t2 = Transformer2D(Mat3x2::Scale(min(1.0, m_startTimer + 0.5)));

	// parts
	for (const auto& p : m_partStates)
	{
		auto t3 = Transformer2D(Mat3x2::Rotate(p->getPartConfig()->getRotation())
			.translated(p->getPartConfig()->getPosition().x, p->getPartConfig()->getPosition().y));

		p->Draw(*this);
	}

	// 細胞円
	if (false)
	{
		double a = min(0.5, m_deathTimer * 0.25);

		Circle(getRadius())
			.draw(ColorF(Palette::Lightpink, a))
			.drawFrame(1.0, Palette::Gray);
	}
}

/*
void CellState::TakeMolecule(const shared_ptr<MoleculeState>& molecule)
{
	m_storage.Add(molecule->getModel());

	molecule->Destroy();
}
*/

/*
void CellState::ExpireMolecule(const shared_ptr<MoleculeModel>& model, unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i)
	{
		// 吐き出す方向
		auto v = Vec2(1.0, 0.0).rotated(rand() / 3600.0);

		// 吐き出されたMoleculeState
		const auto& t = g_moleculeManagerPtr->AddMoleculeState(model);
		t->setPosition(getPosition() + v * (getRadius() + model->getRadius()));
		t->SetVelocity(v * 0.5);

		// Storageから出す
		m_storage.Pull(model);
	}
}
*/