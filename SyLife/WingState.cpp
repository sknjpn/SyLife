#include "WingState.h"

#include "PartConfig.h"

#include "Cell.h"

void WingState::draw(const CellState& cell) const
{
	auto t = Transformer2D(Mat3x2::Rotate(dynamic_pointer_cast<WingModel>(getPartConfig()->getModel())->m_isRight ? m_p : -m_p));

	getPartConfig()->getModel()->draw();
}

void WingState::update(CellState& cell)
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

void WingState::Flap(CellState& cell)
{
	cell.AddImpulseInLocal(Vec2::Up() * 10000.0, getPartConfig()->getPosition());
}
