#pragma once

#include "ModuleState.h"

class SynthesizerState
	: public ModuleState
{
	double	m_timer = 0.0;

public:
	void	Draw(const CellState& cell) const
	{
		GetPartConfig()->getModel()->Draw(min(m_timer / 2.0, 1.0) * 0.75 + 0.25);
	}

	void	Update(CellState& cell) override;
};
