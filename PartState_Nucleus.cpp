#include "PartState_Nucleus.h"
#include "PartAsset_Nucleus.h"

#include "PartConfig.h"

PartState_Nucleus::PartState_Nucleus(const shared_ptr<PartConfig>& partConfig)
	: PartState(partConfig)
	, m_partAsset_Nucleus(dynamic_pointer_cast<PartAsset_Nucleus>(partConfig->getPartAsset()))
{}

void PartState_Nucleus::draw(const CellState& cellState) const
{
	getPartConfig()->getPartAsset()->draw();
}

void PartState_Nucleus::update(CellState& cellState)
{
}
