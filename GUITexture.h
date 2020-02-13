#pragma once

#include "EasyViewer.h"

class GUITexture :
	public EasyViewer
{
public:
	Texture m_texture;
	Color	m_color;

	enum class Mode
	{
		KeepScale,
		KeepAspectRatio,
		Maximize,
	} m_mode = Mode::KeepAspectRatio;

public:
	GUITexture(const Texture& texture, Mode mode = Mode::KeepAspectRatio, const Color& color = ColorF(1.0))
		: m_texture(texture)
		, m_mode(mode)
		, m_color(color)
	{}

	void	init() override
	{
		setIsPenetrated(true);
	}

	void	update() override
	{
		const RectF rect(getViewerSize());

		switch (m_mode)
		{
		case GUITexture::Mode::KeepScale:
			m_texture.drawAt(getViewerSize() / 2.0, m_color);
			break;
		case GUITexture::Mode::KeepAspectRatio:
		{
			const double scale = Min(getViewerSize().x / m_texture.width(), getViewerSize().y / m_texture.height());

			m_texture.scaled(scale).drawAt(getViewerSize() / 2.0, m_color);
		}
		break;
		case GUITexture::Mode::Maximize:
			m_texture.fitted(getViewerSize()).draw(m_color);
			break;
		default:
			break;
		}
	}

};
