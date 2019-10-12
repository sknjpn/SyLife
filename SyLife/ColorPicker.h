#pragma once

class ColorPicker
{
	Texture	m_circleTexture;
	Texture	m_barTexture;

public:
	ColorPicker();

	void	update(const HSV& hsv);
};

