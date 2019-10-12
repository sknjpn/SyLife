#include "ColorPicker.h"

ColorPicker::ColorPicker()
{
	const int length = 64;
	const Size size(length, length);

	// Circle
	{
		Image image;
		for (auto p : step(size))
		{
			if ((p - size / 2).length() < length / 2)
				image[p] = HSV(atan2(p.y, p.x) * Math::Pi, (p - size / 2).length() / (length / 2));
		}
		m_circleTexture = Texture(image);
	}
}

void ColorPicker::update()
{

}
