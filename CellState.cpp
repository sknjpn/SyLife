#include "CellState.h"
#include "CellAsset.h"
#include "World.h"
#include "PartAsset.h"
#include "PartConfig.h"
#include "PartState.h"

#include "ElementAsset.h"
#include "ElementState.h"

#include "EggState.h"

CellState::CellState(const shared_ptr<CellAsset>& cellAsset)
	: m_cellAsset(cellAsset)
	, m_startTimer(0.0)
	, m_deathTimer(cellAsset->getLifespanTime())
	, m_yieldTimer(0.0)
	, m_hitpoint(100.0)
{
	setMass(m_cellAsset->getMass());
	setRadius(m_cellAsset->getRadius());
	setInertia(m_cellAsset->getInertia());

	// parts
	for (const auto& pc : m_cellAsset->getPartConfigs())
		m_partStates.emplace_back(pc->getPartAsset()->makeState())->setPartConfig(pc);
}

void CellState::updateCell()
{
	// 衝突処理
	{
		auto result = World::GetInstance()->getField().getCellStateKDTree().knnSearch(2, getPosition());
		if (result.size() == 2)
		{
			auto& t = World::GetInstance()->getField().getCellStates()[result[1]];

			if (t->getPosition() != getPosition() && (getRadius() + t->getRadius() - (t->getPosition() - getPosition()).length()) > 0)
			{
				auto f = -1000.0 * (t->getPosition() - getPosition()).normalized() * (getRadius() + t->getRadius() - (t->getPosition() - getPosition()).length());
				addForceInWorld(f, getPosition());
				t->addForceInWorld(-f, t->getPosition());
			}
		}
	}

	// Timer
	m_startTimer += DeltaTime;

	// parts
	for (const auto& p : m_partStates) p->update(*this);

	// Nutritionの取り込み
	takeNutrition();

	// 接触したElementStateの取り込み
	for (auto i : World::GetInstance()->getField().getElementStateKDTree().knnSearch(1, getPosition()))
	{
		auto& m = World::GetInstance()->getField().getElementStates()[i];

		if (!m->isDestroyed() &&
			(m->getPosition() - getPosition()).length() - getRadius() < 0.0 &&
			m_cellAsset->getMaxStorage().numElement(m->getPartAsset()) > m_storage.numElement(m->getPartAsset()))
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
			if (2 * m_asset->getMaterial().Num(m.first) < m_storage.Num(m.first))
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
	if (m_yieldTimer > 0)
	{
		if ((m_yieldTimer += DeltaTime) >= m_cellAsset->getYieldTime())
		{
			m_yieldTimer = 0.0;
			m_storage -= m_cellAsset->getMaterial();

			const auto& e = World::GetInstance()->getField().addEggState(m_cellAsset);
			e->setPosition(getPosition());
			e->setRotation(Random(Math::TwoPi));
			e->setVelocity(Vec2(1.0, 0.0).rotated(rand() / 360.0));
		}
	}
	else if (m_storage >= m_cellAsset->getMaterial())
	{
		m_yieldTimer += DeltaTime;
	}

	// 死亡処理
	if ((m_deathTimer -= DeltaTime) <= 0.0)
	{
		// Nutritionの吐き出し
		World::GetInstance()->getField().getChip(getPosition())->addNutrition(m_storage.getNutrition() + m_cellAsset->getMaterial().getNutrition());

		// ElementStateの吐き出し
		auto s = m_storage + m_cellAsset->getMaterial();
		for (const auto& m : s.getElementList())
		{
			for (int i = 0; i < m.second; i++)
			{
				// 吐き出す方向
				auto v = Vec2(1.0, 0.0).rotated(rand() / 3600.0);

				// 吐き出されたElementState
				const auto& ms = World::GetInstance()->getField().addElementState(m.first);
				ms->setPosition(getPosition() + v * (getRadius() + m.first->getRadius()) * Random(1.0));
				ms->setVelocity(v * 0.1);
			}
		}

		destroy();
	}
}

void CellState::draw()
{
	auto t1 = Transformer2D(getMat3x2());
	auto t2 = Transformer2D(Mat3x2::Scale(min(1.0, m_startTimer + 0.5)));

	// parts
	for (const auto& p : m_partStates)
	{
		auto t3 = Transformer2D(p->getPartConfig()->getMat3x2());

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
	const double space = m_cellAsset->getMaxStorage().getNutrition() - m_storage.getNutrition();
	const double amount = World::GetInstance()->getField().getChip(getPosition())->getNutrition();
	const double value = Min(space, amount);

	World::GetInstance()->getField().getChip(getPosition())->pullNutrition(value);
	m_storage.addNutrition(value);
}

void CellState::takeElement(const shared_ptr<ElementState>& elementState)
{
	m_storage.addElement(elementState->getPartAsset());

	elementState->destroy();
}

void CellState::load(Deserializer<ByteArray>& reader)
{
	Rigidbody::load(reader);

	reader >> m_startTimer;
	reader >> m_deathTimer;
	reader >> m_yieldTimer;
	reader >> m_hitpoint;
	
	m_storage.load(reader);

	{
		String cellAssetName;
		reader >> cellAssetName;
		m_cellAsset = World::GetAsset<CellAsset>(cellAssetName);
	}

	// parts
	for (const auto& pc : m_cellAsset->getPartConfigs())
	{
		const auto partState = m_partStates.emplace_back(pc->getPartAsset()->makeState());
		partState->setPartConfig(pc);
		partState->load(reader);
	}

	// 設定
	setMass(m_cellAsset->getMass());
	setRadius(m_cellAsset->getRadius());
	setInertia(m_cellAsset->getInertia());
}

void CellState::save(Serializer<MemoryWriter>& writer) const
{
	Rigidbody::save(writer);

	writer << m_startTimer;
	writer << m_deathTimer;
	writer << m_yieldTimer;
	writer << m_hitpoint;

	m_storage.save(writer);
	
	writer << m_cellAsset->getName();

	for (const auto& partState : m_partStates)
		partState->save(writer);
}
