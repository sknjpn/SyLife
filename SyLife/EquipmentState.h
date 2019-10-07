#pragma once

#include "PartState.h"

class EquipmentState
	: public PartState
{
public:
	void	draw(const CellState& cell) const override;
	void	update(CellState& cell) override;
};