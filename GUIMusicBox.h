#pragma once

#include "EasyViewer.h"
#include "GUIButtonIcon.h"
#include "GUIValuer.h"

class GUIMusicBox : public EasyViewer
{
	Texture	m_textureVolume_mute = Texture(Icon(0xf6a9, 40));
	Texture	m_textureVolume_max = Texture(Icon(0xf028, 40));
	Texture	m_textureVolume_min = Texture(Icon(0xf027, 40));
	Audio	m_audio;
	double	m_volume;
	bool	m_isEnabled = true;

	void onClicked()
	{
		if (m_isEnabled)
		{
			getChildViewer<GUIButtonIcon>()->setIcon(0xf6a9);
			getChildViewer<GUIValuer>()->setValue(0.0);
			m_audio.setVolume(0.0);
		}
		else
		{
			getChildViewer<GUIButtonIcon>()->setIcon(0xf028);
			getChildViewer<GUIValuer>()->setValue(m_volume);
			m_audio.setVolume(m_volume);
		}

		m_isEnabled = !m_isEnabled;
	}

public:
	GUIMusicBox(const FilePath& path)
		: m_audio(path)
	{
		INIData ini(U"config.ini");
		m_volume = ini.getOr<double>(U"General", U"MusicVolume", 1.0);

		m_audio.setLoop(true);
		m_audio.setVolume(m_volume);
		m_audio.play();
	}

	void init() override
	{
		setViewerRectInLocal(20, 20, 60, 75);

		addChildViewer<GUIButtonIcon>(m_volume, [this]() { onClicked(); })
			->setViewerRectInLocal(5, 5, 50, 50);

		getChildViewer<GUIButtonIcon>()->setIcon(0xf028);

		addChildViewer<GUIValuer>(m_volume)
			->setViewerRectInLocal(5, 60, 50, 10);

	}

	void update() override
	{
		RectF(getViewerSize()).rounded(5).draw(Palette::White).drawFrame(1.0, 0.0, Palette::Black);

		if (m_isEnabled)
		{
			m_audio.setVolume(m_volume);

			if (m_volume != getChildViewer<GUIValuer>()->getValue())
			{
				m_volume = getChildViewer<GUIValuer>()->getValue();

				{
					INIData ini(U"config.ini");
					ini.write(U"General", U"MusicVolume", m_volume);
					ini.save(U"config.ini");
				}

				if (m_volume > 0.5) getChildViewer<GUIButtonIcon>()->setIcon(0xf028);
				else getChildViewer<GUIButtonIcon>()->setIcon(0xf027);
			}
		}
		else
		{
			m_audio.setVolume(0.0);
			getChildViewer<GUIValuer>()->setValue(0.0);
		}

		m_textureVolume_max.drawAt(30, 30, Palette::Black);
	}
};
