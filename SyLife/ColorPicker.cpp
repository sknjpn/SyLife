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

void ColorPicker::update(const HSV& hsv)
{
	m_circleTexture.draw(8, 8);
	m_barTexture.draw(76, 8);

	const int length = 64;

	// Circle
	RectF(Vec2(hsv.s * length, 0.0).rotated(ToRadians(hsv.h)), Vec2(10, 10))
		.movedBy(length / 2 + 8, length / 2 + 8)
		.draw(hsv)
		.drawFrame(1.0, Palette::Black);
}
