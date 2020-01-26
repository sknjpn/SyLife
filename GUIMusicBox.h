#pragma once

#include "EasyViewer.h"
#include "GUIButtonIcon.h"
#include "GUIValuer.h"

class GUIMusicBox : public EasyViewer
{
	bool	m_loadComplate = false;
	String	m_assetName;
	double	m_volume;
	bool	m_isEnabled = true;

	void onClicked()
	{
		if (m_isEnabled)
		{
			getChildViewer<GUIButtonIcon>()->setIcon(0xf6a9);
			getChildViewer<GUIValuer>()->setValue(0.0);
			AudioAsset(m_assetName).setVolume(0.0);
		}
		else
		{
			getChildViewer<GUIButtonIcon>()->setIcon(0xf028);
			getChildViewer<GUIValuer>()->setValue(m_volume);
			AudioAsset(m_assetName).setVolume(m_volume);
		}

		m_isEnabled = !m_isEnabled;
	}

public:
	GUIMusicBox(const String& assetName)
		: m_assetName(assetName)
	{
		INIData ini(U"config.ini");
		m_volume = ini.getOr<double>(U"General", U"MusicVolume", 1.0);
	}

	void init() override
	{
		setViewerRectInLocal(20, 20, 60, 75);

		addChildViewer<GUIButtonIcon>([this]() { onClicked(); })
			->setViewerRectInLocal(5, 5, 50, 50);

		getChildViewer<GUIButtonIcon>()->setIcon(0xf028);

		addChildViewer<GUIValuer>(m_volume)
			->setViewerRectInLocal(5, 60, 50, 10);

	}

	void update() override
	{
		if (!m_loadComplate && AudioAsset::IsReady(m_assetName))
		{
			m_loadComplate = true;
			AudioAsset(m_assetName).setLoop(true);
			AudioAsset(m_assetName).setVolume(m_volume);
			AudioAsset(m_assetName).play();
		}

		RectF(getViewerSize()).rounded(5).draw(Palette::White).drawFrame(1.0, 0.0, Palette::Black);

		if (m_isEnabled)
		{
			AudioAsset(m_assetName).setVolume(m_volume);

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
			AudioAsset(m_assetName).setVolume(0.0);
			getChildViewer<GUIValuer>()->setValue(0.0);
		}
	}

	void onDestroy() override
	{
		AudioAsset(m_assetName).stop();
	}
};
