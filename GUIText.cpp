#include "GUIText.h"

void GUIText::update()
{
	const RectF rect(getViewerSize());

	// 文字列描画
	switch (m_mode)
	{
	case GUIText::Mode::DrawAtCenter:
	{
		m_font(m_text).drawAt(rect.center(), m_color);
	}
	break;
	case GUIText::Mode::DrawInBox:
	{
		m_font(m_text).draw(rect, m_color);
	}
	break;
	case GUIText::Mode::DrawLeftCenter:
	{
		m_font(m_text).draw(Arg::leftCenter(0.0, getViewerSize().y / 2.0), m_color);
	}
	break;
	default:
		break;
	}
}
