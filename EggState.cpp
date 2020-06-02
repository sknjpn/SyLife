#include "EggState.h"
#include "CellAsset.h"
#include "CellState.h"
#include "PartConfig.h"
#include "PartAsset.h"
#include "Part_BodyAsset.h"
#include "World.h"

EggState::EggState(const std::shared_ptr<CellAsset>& cellAsset)
	: m_cellAsset(cellAsset)
	, m_timer(m_cellAsset->getBornTime())
{
	setRadius(cellAsset->getRadius());
	setMass(cellAsset->getMass());
	setInertia(cellAsset->getInertia());
}

void EggState::updateEgg()
{
	m_timer -= DeltaTime;

	// 衝突処理
	{
		auto result = World::GetInstance()->getEggStateKDTree().knnSearch(2, getPosition());
		if (result.size() == 2)
		{
			auto& t = World::GetInstance()->getEggStates()[result[1]];

			if (t->getPosition() != getPosition() && (getRadius() + t->getRadius() - (t->getPosition() - getPosition()).length()) > 0)
			{
				auto f = -1000.0 * (t->getPosition() - getPosition()).normalized() * (getRadius() + t->getRadius() - (t->getPosition() - getPosition()).length());
				addForceInWorld(f, getPosition());
				t->addForceInWorld(-f, t->getPosition());
			}
		}
	}

	// 孵化
	if (m_timer <= 0)
	{
		destroy();

		const auto& c = World::GetInstance()->addCellState(getCellAsset());
		c->setPosition(getPosition());
		c->setRotation(getRotation());
	}
}

void EggState::draw1()
{
	const double stage = 1.0 - m_timer / m_cellAsset->getBornTime();

	m_cellAsset->getCellAssetTexture()
		.scaled(1.0 / GeneralSetting::GetInstance().m_textureScale)
		.scaled(Math::Lerp(0.125, 0.15, stage))
		.rotated(getRotation())
		.drawAt(getPosition(), ColorF(1.0, 0.5));
}

void EggState::draw2()
{
	const double stage = 1.0 - m_timer / m_cellAsset->getBornTime();

	Circle(getPosition(), getRadius() * 2.0 * Math::Lerp(0.125, 0.25, stage)).draw(ColorF(Palette::Lightblue, 0.25));
}

void EggState::load(Deserializer<ByteArray>& reader)
{
	Rigidbody::load(reader);

	{
		String cellAssetName;
		reader >> cellAssetName;
		m_cellAsset = World::GetAsset<CellAsset>(cellAssetName);
	}

	reader >> m_timer;
}

void EggState::save(Serializer<MemoryWriter>& writer) const
{
	Rigidbody::save(writer);

	writer << m_cellAsset->getName();
	writer << m_timer;
}
