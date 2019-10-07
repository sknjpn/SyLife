#pragma once

#include "ModuleState.h"

class SynthesizerState
	: public ModuleState
{
	double	m_timer = 0.0;

public:
	void	Draw(const CellState& cell) const override;
	void	Update(CellState& cell) override;
};
