#include "GUIText.h"

void GUIText::update()
{
	static const Font font(128, Typeface::Bold);

	const RectF rect(getViewerSize());

	// 文字列描画
	{
		const auto s = 0.75 * rect.h / 128.0;
		const auto t = Transformer2D(Mat3x2::Scale(s));

		font(m_text).drawAt(rect.center() / s, Palette::Black);
	}
}
