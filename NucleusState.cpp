#include "NucleusState.h"
#include "NucleusAsset.h"

#include "PartConfig.h"

void NucleusState::draw(const CellState& cellState) const
{
	getPartConfig()->getPartAsset()->draw();
}

void NucleusState::update(CellState& cellState)
{
}
