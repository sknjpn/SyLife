#include "PartState_Eye.h"
#include "PartAsset_Eye.h"
#include "PartConfig.h"
#include "PartAsset_Wing.h"
#include "PartState_Wing.h"
#include "CellState.h"
#include "CellAsset.h"
#include "World.h"

PartState_Eye::PartState_Eye(const std::shared_ptr<PartConfig>& partConfig)
	: PartState(partConfig)
	, m_partAsset_Eye(std::dynamic_pointer_cast<PartAsset_Eye>(partConfig->getPartAsset()))
{}

void PartState_Eye::draw(const CellState& cellState) const
{
	getPartConfig()->getPartAsset()->draw();

	if (KeyR.pressed())
		Circle(m_partAsset_Eye->getMaxDistance())
		.draw(ColorF(Palette::Red, 0.1))
		.drawFrame(2.0, Palette::Black);
}

void PartState_Eye::update(CellState& cellState)
{
	m_heat -= DeltaTime;

	if (m_targetCellState && !m_targetCellState->isDestroyed())
	{
		Vec2 delta = m_targetCellState->getPosition() - cellState.getPosition();
		Vec2 direction = Vec2::Up().rotated(cellState.getRotation());

		if (delta.isZero()) return;

		if (delta.cross(direction) > 0)
		{
			for (const auto& partState : cellState.m_partStates)
			{
				if (auto wing = std::dynamic_pointer_cast<PartState_Wing>(partState))
				{
					if (!wing->getPartAsset_Wing()->getIsRight()) wing->stop();
					else wing->move();
				}
			}
		}
		else
		{
			for (const auto& partState : cellState.m_partStates)
			{
				if (auto wing = std::dynamic_pointer_cast<PartState_Wing>(partState))
				{
					if (wing->getPartAsset_Wing()->getIsRight()) wing->stop();
					else wing->move();
				}
			}
		}
	}
	else if (m_heat < 0)
	{
		auto p = cellState.getWorldPosition(getPartConfig()->getPosition());
		for (auto i : World::GetInstance()->getCellStateKDTree().radiusSearch(p, m_partAsset_Eye->getMaxDistance(), true))
		{
			auto& t = World::GetInstance()->getCellStates()[i];

			if (!t->isDestroyed() && t->m_cellAsset != cellState.m_cellAsset)
			{
				m_targetCellState = t;

				return;
			}
		}

		m_heat = 5.0;
	}
}
