#pragma once

#include "PartState.h"

class BodyState
	: public PartState
{
public:
	void	Draw(const CellState& cell) const override;
	void	Update(CellState& cell) override;
};

