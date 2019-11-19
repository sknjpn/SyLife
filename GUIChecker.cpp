#include "GUIChecker.h"

Color GUIChecker::getTargetColor() const
{
	return !m_isEnabled ? Color(64) : m_isGrabbed ? Color(isMouseover() ? 128 : 192) : isMouseover() ? Palette::Skyblue : Palette::White;
}

void GUIChecker::update()
{
	static const Font font(128, Typeface::Bold);

	const RectF rect(getViewerSize());
	const double d = rect.h / 3.0;
	const double frameThickness = rect.h / 12;

	if (m_isEnabled)
	{
		if (m_isGrabbed && isMouseover() && MouseL.up())
			m_value = !m_value;

		if (isMouseover() && MouseL.down()) m_isGrabbed = true;
		if (MouseL.up()) m_isGrabbed = false;
	}

	// ゆっくりの色変化
	m_color = m_color.lerp(getTargetColor(), 0.25);

	// 背景
	rect.rounded(d).draw(m_color).drawFrame(frameThickness, 0.0, Palette::Black);

	// 文字列描画
	if (m_value)
	{
		const auto s = 0.75 * rect.h / 128.0;
		const auto t = Transformer2D(Mat3x2::Scale(s));

		font(U"✔").drawAt(rect.center() / s, Palette::Black);
	}
}

void GUIChecker::setIsEnabled(bool isEnabled)
{
	m_isEnabled = isEnabled;

	if (!m_isEnabled) m_isGrabbed = false;
}
