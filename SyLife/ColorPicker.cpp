#include "ColorPicker.h"

ColorPicker::ColorPicker()
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

HSV ColorPicker::update(const HSV& hsv)
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
		if (d.length() < length / 2 && MouseL.pressed())
		{
			result.h = ToDegrees(atan2(d.y, d.x));
			result.s = d.length() / (length / 2);
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
	}

	return result;
}
