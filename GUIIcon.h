#pragma once

#include "EasyViewer.h"

class GUIButtonIcon : public EasyViewer
{
	Color	m_color;
	uint32	m_code;
	Texture	m_texture;
	double	m_scale;

public:
	GUIButtonIcon(uint32 code)
	{
		setIcon(code);
	}

	void	setIcon(uint32 code) { m_code = code; m_texture = Texture(Icon(m_code, Min(getViewerSize().x, getViewerSize().y))); }

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
