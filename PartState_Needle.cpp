#include "PartState_Needle.h"
#include "PartAsset_Needle.h"
#include "PartConfig.h"
#include "CellState.h"
#include "CellAsset.h"
#include "PartAsset_Nucleus.h"
#include "World.h"

PartState_Needle::PartState_Needle(const std::shared_ptr<PartConfig>& partConfig)
	: PartState(partConfig)
	, m_partAsset_Needle(std::dynamic_pointer_cast<PartAsset_Needle>(partConfig->getPartAsset()))
{}

void PartState_Needle::draw(const CellState& cellState) const
{
	auto t = Transformer2D(Mat3x2::Scale(1.0, Max(m_heat - 4.0, 0.0) * 1.0 + 1.0));

	getPartConfig()->getPartAsset()->draw(Max(m_heat - 4.0, 0.0) * 0.9 + 0.1);
}

void PartState_Needle::update(CellState& cellState)
{
	m_heat -= DeltaTime;

	if (m_heat < 0)
	{
		if (!cellState.isNeedNutrition())
		{
			m_heat = 1.0;

			return;
		}

		m_heat = 5.0;

		auto p = cellState.getWorldPosition(getPartConfig()->getPosition() + Vec2::Up().rotated(getPartConfig()->getRotation()) * 50.0);

		for (auto i : World::GetInstance()->getCellStateKDTree().knnSearch(2, p))
		{
			auto& t = World::GetInstance()->getCellStates()[i];

			if (!t->isDestroyed() && t->getRadius() > (t->getPosition() - p).length() && t->m_cellAsset != cellState.m_cellAsset)
			{
				// armor判定
				if (t->getCellAsset()->getNucleusAsset()->getArmor() <= m_partAsset_Needle->getPenetrating())
				{
					t->destroy();
					cellState.m_storage += t->m_storage;
					cellState.m_storage += t->m_cellAsset->getMaterial();
				}
			}
		}
	}
}

void PartState_Needle::load(Deserializer<ByteArray>& reader)
{
	reader >> m_heat;
}

void PartState_Needle::save(Serializer<MemoryWriter>& writer) const
{
	writer << m_heat;
}
