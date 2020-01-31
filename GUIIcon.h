#pragma once

#include "EasyViewer.h"

class GUIIcon : public EasyViewer
{
	Color	m_color;
	uint32	m_code;
	Texture	m_texture;
	double	m_scale;

public:
	GUIIcon(uint32 code, double scale = 0.8, Color color = Palette::Black)
		: m_color(color)
		, m_scale(scale)
	{
		setIcon(code);
	}

	// Set
	void	setColor(const Color& color) { m_color = color; }
	void	setScale(double scale) { m_scale = scale; }
	void	setIcon(uint32 code) { m_code = code; m_texture = Texture(Icon(m_code, int(m_scale * Min(getViewerSize().x, getViewerSize().y)))); }

	void	init() override
	{
		mouseoverDisable();
	}

	void	update() override
	{
		if (getViewerSize() != m_texture.size()) setIcon(m_code);

		// Icon描画
		m_texture.drawAt(getViewerSize() / 2.0, m_color);
	}
};
