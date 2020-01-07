#include "PartState_Eye.h"
#include "PartAsset_Eye.h"
#include "PartConfig.h"

PartState_Eye::PartState_Eye(const shared_ptr<PartConfig>& partConfig)
	: PartState(partConfig)
	, m_partAsset_Eye(dynamic_pointer_cast<PartAsset_Eye>(partConfig->getPartAsset()))
{}

void PartState_Eye::draw(const CellState& cellState) const
{
	getPartConfig()->getPartAsset()->draw();
}

void PartState_Eye::update(CellState& cellState)
{
}
