#pragma once

#include "EquipmentState.h"

class NeedleState
	: public EquipmentState
{
	double	m_heat = 0.0;

public:
	void	draw(const CellState& cellState) const override;
	void	update(CellState& cellState) override;
};
