#pragma once

#include "ModuleState.h"

class SynthesizerState
	: public ModuleState
{
	double	m_timer = 0.0;

public:
	void	draw(const CellState& cellState) const override;
	void	update(CellState& cellState) override;
};
