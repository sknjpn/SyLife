#include "Part_EyeState.h"
#include "Part_EyeAsset.h"
#include "PartConfig.h"
#include "Part_WingAsset.h"
#include "Part_WingState.h"
#include "CellState.h"
#include "CellAsset.h"
#include "World.h"

Part_EyeState::Part_EyeState(const std::shared_ptr<PartConfig>& partConfig)
	: PartState(partConfig)
	, m_Part_EyeAsset(std::dynamic_pointer_cast<Part_EyeAsset>(partConfig->getPartAsset()))
{}

void Part_EyeState::update(CellState& cellState)
{
	const auto position = cellState.getWorldPosition(getPartConfig()->getPosition());

	m_heat -= DeltaTime;

	if (m_targetCellState && m_targetCellState->getPosition().distanceFrom(position) > m_Part_EyeAsset->getMaxDistance() * 1.25)
		m_targetCellState = nullptr;

	if (m_targetCellState && !m_targetCellState->isDestroyed())
	{
		Vec2 delta = m_targetCellState->getPosition() - cellState.getPosition();
		Vec2 direction = Vec2::Up().rotated(cellState.getRotation());

		if (delta.isZero()) return;

		if (delta.cross(direction) > 0)
		{
			for (const auto& partState : cellState.m_partStates)
			{
				if (auto wing = std::dynamic_pointer_cast<Part_WingState>(partState))
				{
					if (!wing->getPart_WingAsset()->getIsRight()) wing->stop();
					else wing->move();
				}
			}
		}
		else
		{
			for (const auto& partState : cellState.m_partStates)
			{
				if (auto wing = std::dynamic_pointer_cast<Part_WingState>(partState))
				{
					if (wing->getPart_WingAsset()->getIsRight()) wing->stop();
					else wing->move();
				}
			}
		}
	}
	else if (m_heat < 0)
	{
		for (auto i : World::GetInstance()->getCellStateKDTree().radiusSearch(position, m_Part_EyeAsset->getMaxDistance(), SortByDistance::Yes))
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
