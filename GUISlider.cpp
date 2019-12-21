#include "GUISlider.h"

void GUISlider::update()
{
	const double w = getViewerSize().x;
	const double vh = getViewerSize().y;
	const double bh = Min(vh * (vh / m_height), vh);

	const double d = w / 3.0;
	const double frameThickness = w / 12;

	if (isMouseover() && MouseL.down())
	{
		m_isGrabbed = true;

		if (getButton().mouseOver()) m_grabbedPos = m_value; 
		else m_grabbedPos = (Cursor::PosF().y - bh / 2.0) / (vh - bh);
	}

	if (MouseL.up()) m_isGrabbed = false;

	if (m_isGrabbed && vh < m_height)
	{
		m_grabbedPos += Cursor::DeltaF().y / (getViewerSize().y - Min(vh * (vh / m_height), vh));
		m_value = Clamp<double>(m_grabbedPos, 0.0, 1.0);
	}

	// ゆっくりの色変化
	m_color = m_color.lerp(getTargetColor(), 0.25);

	// 背景
	RectF(getViewerSize()).rounded(d).draw(Color(64)).drawFrame(frameThickness, 0.0, Palette::Black);
	getButton().rounded(d).draw(m_color).drawFrame(frameThickness, 0.0, Palette::Black);
}
