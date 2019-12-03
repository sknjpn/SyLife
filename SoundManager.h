#pragma once

#include "Sound.h"

class Sound;

class SoundManager
{
	double	m_masterVolume = 0.3;
	double	m_musicVolume = 0.8;
	double	m_uiVolume = 0.6;
	double	m_environmentVolume = 0.4;

	Array<shared_ptr<Sound>>	m_musics;
	Array<shared_ptr<Sound>>	m_soundEffects;

	shared_ptr<Sound> getMusic(const FilePath& filePath);
	shared_ptr<Sound> getSoundEffect(const FilePath& filePath);
	double	getVolume(SoundType soundType) const;

public:
	double	getMasterVolume() const { return m_masterVolume; }
	double	getMusicVolume() const { return m_musicVolume; }
	double	getUIVolume() const { return m_uiVolume; }
	double	getEnvironmentVolume() const { return m_environmentVolume; }

	void	setMasterVolume(double volume) { m_masterVolume = volume; updateVolume(); }
	void	setMusicVolume(double volume) { m_musicVolume = volume; updateVolume(); }
	void	setUIVolume(double volume) { m_uiVolume = volume; updateVolume(); }
	void	setEnvironmentVolume(double volume) { m_environmentVolume = volume; updateVolume(); }


	// 既存の音楽は停止する
	void	playMusic(const FilePath& filePath);
	void	playSoundEffect(const FilePath& soundEffect, SoundType soundType);

	void	updateVolume();
};

extern unique_ptr<SoundManager>	g_soundManagerPtr;
