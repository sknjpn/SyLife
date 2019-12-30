#include "PartState_Eye.h"
#include "PartAsset_Eye.h"
#include "PartConfig.h"

void PartState_Eye::draw(const CellState& cellState) const
{
	getPartConfig()->getPartAsset()->draw();
}

void PartState_Eye::update(CellState& cellState)
{
}
