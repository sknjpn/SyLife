#pragma once

#include "PartState.h"

class EquipmentState
	: public PartState
{
public:
	void	Draw(const CellState& cell) const { GetPartConfig()->getModel()->Draw(); }
	void	Update(CellState& cell) override {}
};