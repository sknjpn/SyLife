#include "ModuleState.h"
#include "ModuleAsset.h"

#include "PartConfig.h"

void ModuleState::draw(const CellState& cellState) const
{
	getPartConfig()->getPartAsset()->draw();
}

void ModuleState::update(CellState& cellState)
{
}
