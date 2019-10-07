#include "CellState.h"
#include "CellModel.h"

#include "SystemManager.h"
#include "EggManager.h"
#include "CellManager.h"

#include "PartModel.h"
#include "PartConfig.h"
#include "PartState.h"

#include "Egg.h"

#include <boost/math/constants/constants.hpp>

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