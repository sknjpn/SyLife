#include "WingState.h"

#include "WingAsset.h"
#include "PartConfig.h"

#include "CellState.h"

void WingState::draw(const CellState& cellState) const
{
	auto t = Transformer2D(Mat3x2::Rotate(dynamic_pointer_cast<WingAsset>(getPartConfig()->getPartAsset())->getIsRight() ? m_p : -m_p));

	getPartConfig()->getPartAsset()->draw();
}

void WingState::update(CellState& cellState)
{
	if ((m_timer++) == 60)
	{
		m_timer = 0;

		if (m_counter > 0) --m_counter;
		else
		{
			flap(cellState);
			m_v = 0.1;
		}
	}

	if ((rand() % 1000) == 0) m_counter = 3;

	m_p += m_v;
	m_p *= 0.95;
	m_v *= 0.95;
}

void WingState::flap(CellState& cellState)
{
	auto centroid = getPartConfig()->getPartAsset()->getShape().getCentroid().rotated(getPartConfig()->getRotation());
	cellState.addImpulseInLocal(Vec2::Up().rotated(getPartConfig()->getRotation()) * 10000.0, getPartConfig()->getPosition() + centroid);
}
