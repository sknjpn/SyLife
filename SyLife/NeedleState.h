#pragma once

#include "EquipmentState.h"

class NeedleState
	: public EquipmentState
{
	double	m_heat = 0.0;

public:
	void	Draw(const CellState& cell) const;
	void	Update(CellState& cell) override;
};
