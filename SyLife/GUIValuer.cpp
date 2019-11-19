#include "GUIValuer.h"

void GUIValuer::update()
{
	const double w = getViewerSize().x;
	const double h = getViewerSize().y;

	const double d = h / 2.0;
	const double frameThickness = h / 12;

	if (isMouseover() && MouseL.down()) m_isGrabbed = true;
	if (MouseL.up()) m_isGrabbed = false;

	if (m_isGrabbed) m_value = Clamp<double>((Cursor::PosF().x - h / 2) / (w - h), 0.0, 1.0);

	// ゆっくりの色変化
	m_color = m_color.lerp(getTargetColor(), 0.25);

	// 背景
	RectF(getViewerSize()).rounded(d).draw(Color(64)).drawFrame(frameThickness, 0.0, Palette::Black);
	getButton().draw(m_color).drawFrame(frameThickness, 0.0, Palette::Black);
}

Circle GUIValuer::getButton() const
{
	const double w = getViewerSize().x;
	const double h = getViewerSize().y;
	
	return Circle(h / 2 + (w - h) * m_value, h / 2, h / 2);
}

void GUIValuer::init()
{
	m_color = getTargetColor();
}
