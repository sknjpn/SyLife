#pragma once

#include "EquipmentState.h"

class WingState
	: public EquipmentState
{
	int		m_timer = 0;
	double	m_v = 0.0;
	double	m_p = 0.0;
	int		m_counter = 0;

public:
	void	Draw(const CellState& cell) const
	{
		auto t = Transformer2D(Mat3x2::Rotate(dynamic_pointer_cast<WingModel>(GetPartConfig()->getModel())->m_isRight ? m_p : -m_p));

		GetPartConfig()->getModel()->Draw();
	}
	void	Update(CellState& cell) override
	{
		if ((m_timer++) == 60)
		{
			m_timer = 0;

			if (m_counter > 0) --m_counter;
			else
			{
				Flap(cell);
				m_v = 0.1;
			}
		}

		if ((rand() % 1000) == 0) m_counter = 3;

		m_p += m_v;
		m_p *= 0.95;
		m_v *= 0.95;
	}

	void	Flap(CellState& cell);
};
