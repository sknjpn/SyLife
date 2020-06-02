#include "Part_NucleusState.h"
#include "Part_NucleusAsset.h"

#include "PartConfig.h"

Part_NucleusState::Part_NucleusState(const std::shared_ptr<PartConfig>& partConfig)
	: PartState(partConfig)
	, m_Part_NucleusAsset(std::dynamic_pointer_cast<Part_NucleusAsset>(partConfig->getPartAsset()))
{}

void Part_NucleusState::update(CellState& cellState)
{
}
