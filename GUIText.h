#pragma once

#include "Viewer.h"

class GUIText :
	public Viewer
{
public:
	String	m_text;
	Font	m_font;
	Color	m_color;

	enum class Mode
	{
		DrawAtCenter,
		DrawInBox,
		DrawLeftCenter,
	} m_mode;

public:
	GUIText(const String& text, const Font& font, Mode mode = Mode::DrawAtCenter, Color color = Palette::Black)
		: m_text(text)
		, m_font(font)
		, m_mode(mode)
		, m_color(color)
	{}

	void	update() override;
};
