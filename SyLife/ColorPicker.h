#pragma once

class ColorPicker
{
	HSV		m_hsv;
	Texture	m_circleTexture;
	Texture	m_barTexture;

public:
	ColorPicker();

	void	update();
};

