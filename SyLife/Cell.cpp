#include "Cell.h"
#include "Part.h"
#include "Egg.h"

#include <boost/math/constants/constants.hpp>
using namespace boost::math::constants;

void CellModel::Draw(double a)
{
	// parts
	for (const auto& pc : m_partConfigs)
	{
		auto t2 = Transformer2D(Mat3x2::Rotate(pc->GetRotation()).translated(pc->GetPosition()));

		pc->GetModel()->Draw(a);
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

shared_ptr<PartConfig>& CellModel::AddPartConfig()
{
	return m_partConfigs.emplace_back(make_shared<PartConfig>());
}

void CellModel::UpdateProperties()
{
	// mass
	m_mass = accumulate(m_partConfigs.begin(), m_partConfigs.end(), 0.0, [](double mass, const auto& p) { return mass + p->GetModel()->GetMass(); });

	// center
	Vec2 center = accumulate(m_partConfigs.begin(), m_partConfigs.end(), Vec2::Zero(), [](Vec2 acc, const auto& p) { return acc + p->GetModel()->GetMass() * (p->GetPosition() + p->GetModel()->GetCenter().rotated(p->GetRotation())); }) / m_mass;

	// centerを原点に設定
	for (const auto& p : m_partConfigs) p->SetPosition(p->GetPosition() - center);

	// inertia
	m_inertia = accumulate(m_partConfigs.begin(), m_partConfigs.end(), 0.0, [](double acc, const auto& p) { return acc + p->GetInertia(); });

	// radius
	m_radius = sqrt(2 * m_inertia / m_mass);

	// material
	m_material = accumulate(m_partConfigs.begin(), m_partConfigs.end(), Storage(), [](Storage acc, const auto& p) { return acc += p->GetModel()->GetMaterial(); });
}

CellState::CellState(const shared_ptr<CellModel>& model)
	: m_model(model)
	, m_startTimer(0.0)
	, m_deathTimer(25.0)
{
	SetMass(m_model->GetMass());
	SetRadius(m_model->GetRadius());
	SetInertia(m_model->GetInertia());

	// parts
	for (const auto& pc : m_model->GetPartConfigs())
		m_partStates.emplace_back(pc->GetModel()->MakeState())->SetPartConfig(pc);
}

void CellState::UpdateCell()
{
	// 衝突処理
	{
		auto result = g_cellManagerPtr->GetCellStateKDTree().knnSearch(2, GetPosition());
		if (result.size() == 2)
		{
			auto& t = g_cellManagerPtr->GetCellStates()[result[1]];

			if (t->GetPosition() != GetPosition() && (GetRadius() + t->GetRadius() - (t->GetPosition() - GetPosition()).length()) > 0)
			{
				auto f = -1000.0 * (t->GetPosition() - GetPosition()).normalized() * (GetRadius() + t->GetRadius() - (t->GetPosition() - GetPosition()).length());
				AddForceInWorld(f, GetPosition());
				t->AddForceInWorld(-f, t->GetPosition());
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
	for (auto i : g_moleculeManagerPtr->GetMoleculeStateKDTree().knnSearch(1, GetPosition()))
	{
		auto& m = g_moleculeManagerPtr->GetMoleculeStates()[i];

		if (!m->IsDestroyed() && (m->GetPosition() - GetPosition()).length() - GetRadius() < 0.0) TakeMolecule(m);
	}
	*/

	// 余剰のMoleculeStateの投棄
	/*
	for (;;)
	{
		bool flag = true;

		for (const auto& m : m_storage.GetMolecules())
		{
			if (2 * m_model->GetMaterial().Num(m.first) < m_storage.Num(m.first))
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
	if (m_storage >= m_model->GetMaterial())
	{
		m_storage -= m_model->GetMaterial();

		const auto& e = g_eggManagerPtr->AddEggState(m_model);
		e->SetPosition(GetPosition());
		e->SetRotation(Random(boost::math::constants::pi<double>() * 2.0));
		e->SetVelocity(Vec2(1.0, 0.0).rotated(rand() / 360.0));
	}

	// 死亡処理
	if (m_deathTimer <= 0.0)
	{
		// MoleculeStateの吐き出し
		/*
		auto s = m_storage + m_model->GetMaterial();
		for (const auto& m : s.GetMolecules())
		{
			for (unsigned int i = 0; i < m.second; i++)
			{
				// 吐き出す方向
				auto v = Vec2(1.0, 0.0).rotated(rand() / 3600.0);

				// 吐き出されたMoleculeState
				const auto& ms = g_moleculeManagerPtr->AddMoleculeState(m.first);
				ms->SetPosition(GetPosition() + v * (GetRadius() + m.first->GetRadius()) * Random(1.0));
				ms->SetVelocity(v * 0.1);
			}
		}
		*/

		Destroy();
	}
}

void CellState::Draw()
{
	auto t1 = Transformer2D(Mat3x2::Rotate(GetRotation()).translated(Vec2(GetPosition().x, GetPosition().y)));
	auto t2 = Transformer2D(Mat3x2::Scale(min(1.0, m_startTimer + 0.5)));

	// parts
	for (const auto& p : m_partStates)
	{
		auto t3 = Transformer2D(Mat3x2::Rotate(p->GetPartConfig()->GetRotation())
			.translated(p->GetPartConfig()->GetPosition().x, p->GetPartConfig()->GetPosition().y));

		p->Draw(*this);
	}

	// 細胞円
	if (false)
	{
		double a = min(0.5, m_deathTimer * 0.25);

		Circle(GetRadius())
			.draw(ColorF(Palette::Lightpink, a))
			.drawFrame(1.0, Palette::Gray);
	}
}

/*
void CellState::TakeMolecule(const shared_ptr<MoleculeState>& molecule)
{
	m_storage.Add(molecule->GetModel());

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
		t->SetPosition(GetPosition() + v * (GetRadius() + model->GetRadius()));
		t->SetVelocity(v * 0.5);

		// Storageから出す
		m_storage.Pull(model);
	}
}
*/