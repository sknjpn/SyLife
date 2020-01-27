#include "PartState_Body.h"
#include "PartAsset_Body.h"
#include "PartConfig.h"

PartState_Body::PartState_Body(const std::shared_ptr<PartConfig>& partConfig)
	: PartState(partConfig)
	, m_partAsset_Body(std::dynamic_pointer_cast<PartAsset_Body>(partConfig->getPartAsset()))
{}

void PartState_Body::draw(const CellState& cellState) const
{
	getPartConfig()->getPartAsset()->draw();
}

void PartState_Body::update(CellState& cellState)
{
}
