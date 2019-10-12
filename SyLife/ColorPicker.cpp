#include "ColorPicker.h"

ColorPicker::ColorPicker()
{
	const int length = 64;
	const Size size(length, length);

	// Circle
	{
		Image image(size);
		for (auto p : step(size))
		{
			if ((p - size / 2).length() < length / 2)
				image[p] = HSV(ToDegrees(atan2(p.y, p.x)), (p - size / 2).length() / (length / 2), 1.0);
		}
		m_circleTexture = Texture(image);
	}
}

void ColorPicker::update()
{
	m_circleTexture.draw();
}
