#pragma once

class GeneralSetting
{
	GeneralSetting()
	{
		load();
	}

public:
	static GeneralSetting& GetInstance()
	{
		static GeneralSetting g_instance;

		return g_instance;
	}

	bool	m_touchPanelModeEnabled;
	bool	m_autoTurnOutEnabled;
	double	m_autoTurnOutTime;
	bool	m_fullScreenEnabled;
	bool	m_runTitleEnabled;
	bool	m_audioEnabled;
	double	m_musicVolume;
	Size	m_sceneSize;

	void	load()
	{
		INIData ini(U"config.ini");
		m_touchPanelModeEnabled = ini.getOr<bool>(U"General", U"TouchPanelModeEnabled", false);
		m_autoTurnOutEnabled = ini.getOr<bool>(U"General", U"AutoTurnOutEnabled", false);
		m_autoTurnOutTime = ini.getOr<double>(U"General", U"AutoTurnOutTime", 10);
		m_fullScreenEnabled = ini.getOr<bool>(U"General", U"FullScreenEnabled", false);
		m_runTitleEnabled = ini.getOr<bool>(U"General", U"RunTitleEnabled", true);
		m_audioEnabled = ini.getOr<bool>(U"General", U"AudioEnabled", true);
		m_musicVolume = ini.getOr<double>(U"General", U"MusicVolume", 1.0);
		m_sceneSize = ini.getOr<Size>(U"General", U"SceneSize", Size(1920, 1080));
	}

	void	save() const
	{
		INIData ini(U"config.ini");	// 追記の形で書き込む
		ini.write<bool>(U"General", U"TouchPanelModeEnabled", m_touchPanelModeEnabled);
		ini.write<bool>(U"General", U"AutoTurnOutEnabled", m_autoTurnOutEnabled);
		ini.write<double>(U"General", U"AutoTurnOutTime", m_autoTurnOutTime);
		ini.write<bool>(U"General", U"FullScreenEnabled", m_fullScreenEnabled);
		ini.write<bool>(U"General", U"RunTitleEnabled", m_runTitleEnabled);
		ini.write<bool>(U"General", U"AudioEnabled", m_audioEnabled);
		ini.write<double>(U"General", U"MusicVolume", m_musicVolume);
		ini.write<Size>(U"General", U"SceneSize", m_sceneSize);
		ini.save(U"config.ini");
	}
};