#pragma once

class ColorPicker
{
	Texture	m_circleTexture;
	Texture	m_barTexture;

public:
	ColorPicker();

	HSV	update(const HSV& hsv);
};

