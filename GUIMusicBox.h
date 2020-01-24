#pragma once

#include "EasyViewer.h"
#include "GUIValuer.h"

class GUIMusicBox : public EasyViewer
{
	Texture	m_textureVolume_mute = Texture(Icon(0xf6a9, 30));
	Texture	m_textureVolume_max = Texture(Icon(0xf028, 30));
	Texture	m_textureVolume_min = Texture(Icon(0xf027, 30));
	Audio	m_audio;
	double	m_volume;

public:
	GUIMusicBox(const FilePath& path, double volume)
		: m_audio(path)
		, m_volume(volume)
	{
		m_audio.setLoop(true);
		m_audio.setVolume(volume);
	}

	void init() override
	{
		setViewerRectInLocal(20, 20, 40, 50);

		addChildViewer<GUIValuer>(m_volume)
			->setViewerRectInLocal(0, 40, 40, 10);
	}


	void update() override
	{
		m_volume = getChildViewer<GUIValuer>()->getValue();

		m_audio.setVolume(m_volume);

		m_textureVolume_max.drawAt(getViewerSize() / 2.0);
	}
};
