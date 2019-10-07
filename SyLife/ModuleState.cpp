#include "ModuleState.h"
#include "ModuleModel.h"

#include "PartConfig.h"

void ModuleState::Draw(const CellState& cell) const
{
	getPartConfig()->getModel()->Draw();
}

void ModuleState::Update(CellState& cell)
{
}
