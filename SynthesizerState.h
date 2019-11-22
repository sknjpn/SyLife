#pragma once

#include "PartState.h"

class SynthesizerState
	: public PartState
{
	double	m_timer = 0.0;

public:
	void	draw(const CellState& cellState) const override;
	void	update(CellState& cellState) override;
};
