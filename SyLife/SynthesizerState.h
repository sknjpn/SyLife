#pragma once

#include "ModuleState.h"

class SynthesizerState
	: public ModuleState
{
	double	m_timer = 0.0;

public:
	void	draw(const CellState& cell) const override;
	void	update(CellState& cell) override;
};
