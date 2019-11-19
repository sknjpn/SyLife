#pragma once

#include "PartState.h"

class ModuleState
	: public PartState
{
public:
	void	draw(const CellState& cellState) const override;
	void	update(CellState& cellState) override;
};
