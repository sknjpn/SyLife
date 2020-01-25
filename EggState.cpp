﻿#include "EggState.h"
#include "CellAsset.h"
#include "CellState.h"
#include "PartConfig.h"
#include "PartAsset.h"
#include "PartAsset_Body.h"
#include "World.h"

EggState::EggState(const std::shared_ptr<CellAsset>& cellAsset)
	: m_cellAsset(cellAsset)
	, m_timer(m_cellAsset->getBornTime())
{
	setRadius(cellAsset->getRadius() / 2.0);
	setMass(cellAsset->getMass() / 4.0);
	setInertia(getMass() * getRadius() * getRadius() / 2.0);
}

void EggState::updateEgg()
{
	m_timer -= DeltaTime;

	// 孵化
	if (m_timer <= 0)
	{
		destroy();

		const auto& c = World::GetInstance()->addCellState(getCellAsset());
		c->setPosition(getPosition());
		c->setRotation(getRotation());
	}
}

void EggState::draw()
{
	const double stage = 1.0 - m_timer / m_cellAsset->getBornTime();

	{
		const auto t1 = Transformer2D(getMat3x2());
		const auto t2 = Transformer2D(Mat3x2::Scale(Clamp(stage * 2, 0.2, 1.0)));

		Circle(getRadius()).draw(ColorF(Palette::Lightblue, 0.4 * Clamp(2.0 - stage * 2.0, 0.0, 1.0)));
	}

	{
		const auto t1 = Transformer2D(getMat3x2());
		const auto t2 = Transformer2D(Mat3x2::Scale(0.5 * Clamp(stage * 2, 0.2, 1.0)));

		for (const auto& partConfig : m_cellAsset->getPartConfigs())
		{
			const auto t3 = Transformer2D(partConfig->getMat3x2());
			const auto& partAsset = partConfig->getPartAsset();

			if (std::dynamic_pointer_cast<PartAsset_Body>(partAsset))
			{
				partAsset->draw(0.5 * Clamp(stage, 0.0, 1.0));
			}
			else
			{
				partAsset->draw(0.5 * Clamp(stage * 2 - 1.0, 0.0, 1.0));
			}
		}
	}
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
