#include "EquipmentState.h"
#include "EquipmentModel.h"

#include "PartConfig.h"

void EquipmentState::Draw(const CellState& cell) const
{
	getPartConfig()->getModel()->Draw();
}

void EquipmentState::Update(CellState& cell)
{
}
