﻿#pragma once

#include "PartState.h"

class PartState_Wing
	: public PartState
{
	int		m_timer = 0;
	double	m_v = 0.0;
	double	m_p = 0.0;
	int		m_counter = 0;

public:
	void	draw(const CellState& cellState) const override;
	void	update(CellState& cellState) override;

	void	flap(CellState& cellState);
};