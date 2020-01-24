#pragma once

#include "EasyViewer.h"
#include "GUIValuer.h"

class GUIMusicBox : public EasyViewer
{
	Texture	m_textureVolume_mute = Texture(Icon(0xf6a9, 40));
	Texture	m_textureVolume_max = Texture(Icon(0xf028, 40));
	Texture	m_textureVolume_min = Texture(Icon(0xf027, 40));
	Audio	m_audio;
	double	m_volume;

public:
	GUIMusicBox(const FilePath& path, double volume)
		: m_audio(path)
		, m_volume(volume)
	{
		m_audio.setLoop(true);
		m_audio.setVolume(volume);
		m_audio.play();
	}

	void init() override
	{
		setViewerRectInLocal(20, 20, 60, 75);

		addChildViewer<GUIValuer>(m_volume)
			->setViewerRectInLocal(0, 60, 60, 15);
	}


	void update() override
	{
		RectF(getViewerSize()).rounded(5).draw(Palette::White).drawFrame(1.0, 0.0, Palette::Black);

		m_volume = getChildViewer<GUIValuer>()->getValue();

		m_audio.setVolume(m_volume);

		m_textureVolume_max.drawAt(30, 30, Palette::Black);
	}
};
