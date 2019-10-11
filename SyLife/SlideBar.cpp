#include "SlideBar.h"

void SlideBar::update()
{
	RectF(30, m_size).stretched(-2).draw(ColorF(0.25)).drawFrame(1.0, 0.0, Palette::White);
	auto bar = RectF(0, getBarPositionY(), 30, m_length).stretched(-4);

	Print << m_value;
	if (m_isDragged)
	{
		if (MouseL.up()) m_isDragged = false;
		else
		{
			m_value = Clamp<double>(m_value + Cursor::DeltaF().y / (m_size - m_length), 0.0, 1.0);
		}

		bar.draw(ColorF(1.0, 1.0));
	}
	else
	{
		bar.draw(bar.mouseOver() ? ColorF(1.0, 0.75) : ColorF(1.0, 0.5));

		if (bar.leftClicked()) m_isDragged = true;
	}
}
