#include "ModuleState.h"
#include "ModuleAsset.h"

#include "PartConfig.h"

void ModuleState::draw(const CellState& cell) const
{
	getPartConfig()->getModel()->draw();
}

void ModuleState::update(CellState& cell)
{
}
