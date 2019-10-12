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
		}
		m_circleTexture = Texture(image);
	}

	// Bar
	{
		const Size size(length / 8, length);
		Image image(size);
		for (auto p : step(size))
		{
			image[p] = 
		}
	}
}

void ColorPicker::update()
{
	m_circleTexture.draw();
}
