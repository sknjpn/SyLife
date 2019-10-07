#include "ModuleState.h"
#include "ModuleModel.h"

#include "PartConfig.h"

void ModuleState::draw(const CellState& cell) const
{
	getPartConfig()->getModel()->draw();
}

void ModuleState::update(CellState& cell)
{
}
