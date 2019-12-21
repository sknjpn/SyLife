#include "PartState_Nucleus.h"
#include "PartAsset_Nucleus.h"

#include "PartConfig.h"

void PartState_Nucleus::draw(const CellState& cellState) const
{
	getPartConfig()->getPartAsset()->draw();
}

void PartState_Nucleus::update(CellState& cellState)
{
}
