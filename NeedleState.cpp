﻿#include "NeedleState.h"

#include "CellManager.h"

#include "NeedleAsset.h"
#include "PartConfig.h"
#include "CellState.h"
#include "CellAsset.h"

void NeedleState::draw(const CellState& cellState) const
{
	auto t = Transformer2D(Mat3x2::Scale(1.0, max(m_heat - 4.0, 0.0) * 1.0 + 1.0));

	getPartConfig()->getPartAsset()->draw(max(m_heat - 4.0, 0.0) * 0.9 + 0.1);
}

void NeedleState::update(CellState& cellState)
{
	m_heat -= DeltaTime;

	if (m_heat < 0)
	{
		m_heat = 5.0;

		auto p = cellState.getWorldPosition(getPartConfig()->getPosition() + Vec2::Up().rotated(getPartConfig()->getRotation()) * 50.0);


		for (auto i : g_cellManagerPtr->getCellStateKDTree().knnSearch(1, p))
		{
			auto& t = g_cellManagerPtr->getCellStates()[i];

			if (!t->isDestroyed() && t->getRadius() > (t->getPosition() - p).length() && t->m_cellAsset != cellState.m_cellAsset)
			{
				t->destroy();
				cellState.m_storage += t->m_storage;
				cellState.m_storage += t->m_cellAsset->getMaterial();
			}
		}
	}
}