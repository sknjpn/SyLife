﻿#pragma once

#include "PartState.h"

class PartState_Needle
	: public PartState
{
	double	m_heat = 0.0;

public:
	void	draw(const CellState& cellState) const override;
	void	update(CellState& cellState) override;
};