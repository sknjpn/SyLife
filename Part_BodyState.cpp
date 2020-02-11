#include "Part_BodyState.h"
#include "Part_BodyAsset.h"
#include "PartConfig.h"

Part_BodyState::Part_BodyState(const std::shared_ptr<PartConfig>& partConfig)
	: PartState(partConfig)
	, m_Part_BodyAsset(std::dynamic_pointer_cast<Part_BodyAsset>(partConfig->getPartAsset()))
{}

void Part_BodyState::update(CellState& cellState)
{
}
