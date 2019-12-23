#pragma once

#include "stdafx.h"

class ColorPicker
{
	Texture	m_circleTexture;
	Texture	m_barTexture;
	bool	m_circleSelected = false;
	bool	m_barSelected = false;

public:
	ColorPicker();

	HSV	update(const HSV& hsv);
};

