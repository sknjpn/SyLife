#pragma once

#include "EquipmentState.h"

class NeedleState
	: public EquipmentState
{
	double	m_heat = 0.0;

public:
	void	draw(const CellState& cell) const override;
	void	update(CellState& cell) override;
};
