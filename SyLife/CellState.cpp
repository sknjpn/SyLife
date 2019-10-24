#include "CellState.h"
#include "CellAsset.h"

#include "SystemManager.h"
#include "EggManager.h"
#include "CellManager.h"
#include "ChipManager.h"
#include "ElementManager.h"

#include "PartAsset.h"
#include "PartConfig.h"
#include "PartState.h"

#include "ElementAsset.h"
#include "ElementState.h"

#include "EggState.h"

#include <boost/math/constants/constants.hpp>

CellState::CellState(const shared_ptr<CellAsset>& model)
	: m_model(model)
	, m_startTimer(0.0)
	, m_deathTimer(25.0)
{
	setMass(m_model->getMass());
	setRadius(m_model->getRadius());
	setInertia(m_model->getInertia());

	// parts
	for (const auto& pc : m_model->getPartConfigs())
		m_partStates.emplace_back(pc->getModel()->makeState())->setPartConfig(pc);
}

void CellState::updateCell()
{
	// 衝突処理
	{
		auto result = g_cellManagerPtr->getCellStateKDTree().knnSearch(2, getPosition());
		if (result.size() == 2)
		{
			auto& t = g_cellManagerPtr->getCellStates()[result[1]];

			if (t->getPosition() != getPosition() && (getRadius() + t->getRadius() - (t->getPosition() - getPosition()).length()) > 0)
			{
				auto f = -1000.0 * (t->getPosition() - getPosition()).normalized() * (getRadius() + t->getRadius() - (t->getPosition() - getPosition()).length());
				addForceInWorld(f, getPosition());
				t->addForceInWorld(-f, t->getPosition());
			}
		}
	}

	// Timer
	m_deathTimer -= g_systemManagerPtr->GetDeltaTime();
	m_startTimer += g_systemManagerPtr->GetDeltaTime();
	if (m_yieldTimer > 0) m_yieldTimer += g_systemManagerPtr->GetDeltaTime();

	// parts
	for (const auto& p : m_partStates) p->update(*this);

	// Nutritionの取り込み
	takeNutrition();

	// 接触したElementStateの取り込み
	for (auto i : g_elementManagerPtr->getElementStateKDTree().knnSearch(1, getPosition()))
	{
		auto& m = g_elementManagerPtr->getElementStates()[i];

		if (!m->isDestroyed() &&
			(m->getPosition() - getPosition()).length() - getRadius() < 0.0 &&
			m_model->getMaxStorage().numElement(m->getModel()) > m_storage.numElement(m->getModel()))
		{
			takeElement(m);
		}
	}

	// 余剰のElementStateの投棄
	/*
	for (;;)
	{
		bool flag = true;

		for (const auto& m : m_storage.GetElements())
		{
			if (2 * m_model->getMaterial().Num(m.first) < m_storage.Num(m.first))
			{
				ExpireElement(m.first);

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

		const auto& e = g_eggManagerPtr->addEggState(m_model);
		e->setPosition(getPosition());
		e->setRotation(Random(boost::math::constants::pi<double>() * 2.0));
		e->setVelocity(Vec2(1.0, 0.0).rotated(rand() / 360.0));
	}

	// 死亡処理
	if (m_deathTimer <= 0.0)
	{
		// Nutritionの吐き出し
		g_chipManagerPtr->addNutrition(getPosition(), m_storage.getNutrition() + m_model->getMaterial().getNutrition());

		// ElementStateの吐き出し
		auto s = m_storage + m_model->getMaterial();
		for (const auto& m : s.getElementList())
		{
			for (int i = 0; i < m.second; i++)
			{
				// 吐き出す方向
				auto v = Vec2(1.0, 0.0).rotated(rand() / 3600.0);

				// 吐き出されたElementState
				const auto& ms = g_elementManagerPtr->addElementState(m.first);
				ms->setPosition(getPosition() + v * (getRadius() + m.first->getRadius()) * Random(1.0));
				ms->setVelocity(v * 0.1);
			}
		}

		destroy();
	}
}

void CellState::draw()
{
	auto t1 = Transformer2D(Mat3x2::Rotate(getRotation()).translated(Vec2(getPosition().x, getPosition().y)));
	auto t2 = Transformer2D(Mat3x2::Scale(min(1.0, m_startTimer + 0.5)));

	// parts
	for (const auto& p : m_partStates)
	{
		auto t3 = Transformer2D(Mat3x2::Rotate(p->getPartConfig()->getRotation())
			.translated(p->getPartConfig()->getPosition().x, p->getPartConfig()->getPosition().y));

		p->draw(*this);
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

void CellState::takeNutrition()
{
	const double space = m_model->getMaxStorage().getNutrition() - m_storage.getNutrition();
	const double amount = g_chipManagerPtr->getNutrition(getPosition());
	const double value = Min(space, amount);

	g_chipManagerPtr->pullNutrition(getPosition(), value);
	m_storage.addNutrition(value);
}

void CellState::takeElement(const shared_ptr<ElementState>& element)
{
	m_storage.addElement(element->getModel());

	element->destroy();
}

/*
void CellState::ExpireElement(const shared_ptr<ElementAsset>& model, unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i)
	{
		// 吐き出す方向
		auto v = Vec2(1.0, 0.0).rotated(rand() / 3600.0);

		// 吐き出されたElementState
		const auto& t = g_elementManagerPtr->addElementState(model);
		t->setPosition(getPosition() + v * (getRadius() + model->getRadius()));
		t->setVelocity(v * 0.5);

		// Storageから出す
		m_storage.Pull(model);
	}
}
*/