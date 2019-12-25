#include "MainViewer.h"

MainViewer::CellMakingViewer::BodySculptor::LayerLists::ColorPicker::ColorPicker()
{
	const int length = 64;

	// Circle
	{
		const Size size(length, length);
		Image image(size);
		for (auto p : step(size))
		{
			const Point d = (p - size / 2);

			if (d.length() < length / 2)
				image[p] = HSV(ToDegrees(atan2(d.y, d.x)), d.length() / (length / 2), 1.0);
			else
				image[p] = Color(0, 0);
		}
		m_circleTexture = Texture(image);
	}

	// Bar
	{
		const Size size(length / 8, length);
		Image image(size);
		for (auto p : step(size))
		{
			image[p] = ColorF(1.0 - p.y / double(length));
		}
		m_barTexture = Texture(image);
	}
}

HSV MainViewer::CellMakingViewer::BodySculptor::LayerLists::ColorPicker::update(const HSV& hsv)
{
	HSV result = hsv;

	const int length = 64;

	// Circle
	{
		auto t = Transformer2D(Mat3x2::Translate(8, 8), true);

		m_circleTexture.draw();

		RectF(Vec2(10, 10))
			.setCenter(Vec2(hsv.s * length / 2.0, 0.0).rotated(ToRadians(hsv.h)))
			.movedBy(length / 2, length / 2)
			.drawFrame(1.0, Palette::Black);

		auto d = Cursor::PosF() - Vec2(length / 2.0, length / 2.0);
		if (Circle(Vec2(32, 32), 32).leftClicked()) m_circleSelected = true;
		if (m_circleSelected)
		{
			result.h = ToDegrees(atan2(d.y, d.x));
			result.s = Min(1.0, d.length() / (length / 2));
		}
	}

	// Bar
	{
		auto t = Transformer2D(Mat3x2::Translate(76, 8), true);

		m_barTexture.draw();

		Triangle(Vec2(0, 0), Vec2(8, -4), Vec2(8, 4))
			.movedBy(length / 8, (1.0 - hsv.v) * length)
			.draw()
			.drawFrame(1.0, Palette::Black);

		if (Rect(8, 64).leftClicked()) m_barSelected = true;
		if (m_barSelected)
			result.v = Clamp<double>(1.0 - Cursor::PosF().y / length, 0.05, 1.0);	// 完全に0にするとSも0になるので注意
	}

	if (MouseL.up())
	{
		m_barSelected = false;
		m_circleSelected = false;
	}

	return result;
}
