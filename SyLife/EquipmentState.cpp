#include "EquipmentState.h"
#include "EquipmentModel.h"

#include "PartConfig.h"

void EquipmentState::draw(const CellState& cell) const
{
	getPartConfig()->getModel()->draw();
}

void EquipmentState::update(CellState& cell)
{
}
