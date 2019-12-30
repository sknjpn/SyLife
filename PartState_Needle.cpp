#include "PartState_Needle.h"
#include "PartAsset_Needle.h"
#include "PartConfig.h"
#include "CellState.h"
#include "CellAsset.h"
#include "World.h"

void PartState_Needle::draw(const CellState& cellState) const
{
	auto t = Transformer2D(Mat3x2::Scale(1.0, max(m_heat - 4.0, 0.0) * 1.0 + 1.0));

	getPartConfig()->getPartAsset()->draw(max(m_heat - 4.0, 0.0) * 0.9 + 0.1);
}

void PartState_Needle::update(CellState& cellState)
{
	m_heat -= DeltaTime;

	if (m_heat < 0)
	{
		m_heat = 5.0;

		auto p = cellState.getWorldPosition(getPartConfig()->getPosition() + Vec2::Up().rotated(getPartConfig()->getRotation()) * 50.0);


		for (auto i : World::GetInstance()->getCellStateKDTree().knnSearch(2, p))
		{
			auto& t = World::GetInstance()->getCellStates()[i];

			if (!t->isDestroyed() && t->getRadius() > (t->getPosition() - p).length() && t->m_cellAsset != cellState.m_cellAsset)
			{
				if (t->addDamage(dynamic_pointer_cast<PartAsset_Needle>(getPartConfig()->getPartAsset())->getDamage()))
				{
					t->destroy();
					cellState.m_storage += t->m_storage;
					cellState.m_storage += t->m_cellAsset->getMaterial();
				}
			}
		}
	}
}
