#pragma once

#include "EasyViewer.h"

class GUIIcon : public EasyViewer
{
	Color	m_color;
	uint32	m_code;
	Texture	m_texture;
	double	m_scale;

public:
	GUIIcon(uint32 code, double scale = 0.75, Color color = Palette::Black)
		: m_color(color)
		, m_scale(scale)
		, m_code(code)
	{}

	// Get
	Color	getColor() const { return m_color; }

	// Set
	void	setColor(const Color& color) { m_color = color; }
	void	setScale(double scale) { m_scale = scale; }
	// TODO:fix texture load
	void	setIcon(uint32 code) { m_code = code; m_texture = Texture(Icon(m_code, int(m_scale * Min(getViewerSize().x, getViewerSize().y))));*/ }

	void	init() override
	{
		setIcon(m_code);
		setIsPenetrated(true);
	}

	void	update() override
	{
		// Icon描画
		m_texture.drawAt(getViewerSize() / 2.0, m_color);
	}
};
