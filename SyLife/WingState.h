﻿#pragma once

#include "EquipmentState.h"

class WingState
	: public EquipmentState
{
	int		m_timer = 0;
	double	m_v = 0.0;
	double	m_p = 0.0;
	int		m_counter = 0;

public:
	void	draw(const CellState& cell) const override;
	void	update(CellState& cell) override;

	void	Flap(CellState& cell);
};
