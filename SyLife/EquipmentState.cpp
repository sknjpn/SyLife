#include "EquipmentState.h"
#include "EquipmentAsset.h"

#include "PartConfig.h"

void EquipmentState::draw(const CellState& cellState) const
{
	getPartConfig()->getModel()->draw();
}

void EquipmentState::update(CellState& cellState)
{
}
