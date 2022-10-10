#pragma once

class GeneralSetting
{
  GeneralSetting() { load(); }

public:
  static GeneralSetting& GetInstance()
  {
    static GeneralSetting g_instance;

    return g_instance;
  }

  bool   m_touchPanelModeEnabled;
  bool   m_autoTurnOutEnabled;
  double m_autoTurnOutTime;
  bool   m_fullScreenEnabled;
  bool   m_runTitleEnabled;
  bool   m_audioEnabled;
  double m_musicVolume;
  double m_textureScale;
  int    m_assetIconSize;
  Size   m_sceneSize;

  void load()
  {
    INI ini(U"config.ini");
    m_touchPanelModeEnabled = ini.getOr<bool>(U"General", U"TouchPanelModeEnabled", false);
    m_autoTurnOutEnabled = ini.getOr<bool>(U"General", U"AutoTurnOutEnabled", false);
    m_autoTurnOutTime = ini.getOr<double>(U"General", U"AutoTurnOutTime", 10);
    m_fullScreenEnabled = ini.getOr<bool>(U"General", U"FullScreenEnabled", false);
    m_runTitleEnabled = ini.getOr<bool>(U"General", U"RunTitleEnabled", true);
    m_audioEnabled = ini.getOr<bool>(U"General", U"AudioEnabled", true);
    m_musicVolume = ini.getOr<double>(U"General", U"MusicVolume", 1.0);
    m_textureScale = ini.getOr<double>(U"General", U"TextureScale", 4.0);
    m_assetIconSize = ini.getOr<int>(U"General", U"AssetIconSize", 256);
    m_sceneSize = ini.getOr<Size>(U"General", U"SceneSize", Size(2560, 1440));
  }

  void save() const
  {
    INI ini(U"config.ini"); // 追記の形で書き込む
    ini.write<bool>(U"General", U"TouchPanelModeEnabled",
        m_touchPanelModeEnabled);
    ini.write<bool>(U"General", U"AutoTurnOutEnabled", m_autoTurnOutEnabled);
    ini.write<double>(U"General", U"AutoTurnOutTime", m_autoTurnOutTime);
    ini.write<bool>(U"General", U"FullScreenEnabled", m_fullScreenEnabled);
    ini.write<bool>(U"General", U"RunTitleEnabled", m_runTitleEnabled);
    ini.write<bool>(U"General", U"AudioEnabled", m_audioEnabled);
    ini.write<double>(U"General", U"MusicVolume", m_musicVolume);
    ini.write<double>(U"General", U"TextureScale", m_textureScale);
    ini.write<int>(U"General", U"AssetIconSize", m_assetIconSize);
    ini.write<Size>(U"General", U"SceneSize", m_sceneSize);
    ini.save(U"config.ini");
  }
};
