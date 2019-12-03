#include "SoundManager.h"
#include "Sound.h"

unique_ptr<SoundManager>	g_soundManagerPtr;

shared_ptr<Sound> SoundManager::getMusic(const FilePath& filePath)
{
	for (auto& music : m_musics)
		if (music->getFilePath() == filePath) return music;

	return nullptr;
}

shared_ptr<Sound> SoundManager::getSoundEffect(const FilePath& filePath)
{
	for (auto& soundEffect : m_soundEffects)
		if (soundEffect->getFilePath() == filePath) return soundEffect;

	return nullptr;
}

double SoundManager::getVolume(SoundType soundType) const
{
	switch (soundType)
	{
	case SoundType::Music: return m_masterVolume * m_musicVolume;
	case SoundType::UI: return m_masterVolume * m_uiVolume;
	case SoundType::Environment: return m_masterVolume * m_environmentVolume;
	}
}

void SoundManager::playMusic(const FilePath& filePath)
{
	{
		auto music = getMusic(filePath);
		if (!music) music = m_musics.emplace_back(MakeShared<Sound>(filePath, SoundType::Music));

		music->setLoop(true);
		music->setVolume(getVolume(SoundType::Music));
	}

	for (const auto& music : m_musics)
		music->stop();

	getMusic(filePath)->play();
}

void SoundManager::playSoundEffect(const FilePath& filePath, SoundType soundType)
{
	{
		auto soundEffect = getSoundEffect(filePath);

		if (!soundEffect) soundEffect = m_soundEffects.emplace_back(MakeShared<Sound>(filePath, soundType));
	}

	for (const auto& soundEffect : m_soundEffects)
		if (soundEffect->getFilePath() == filePath)
			soundEffect->playOneShot(getVolume(soundType));
}

void SoundManager::updateVolume()
{
	for (const auto& m : m_musics) m->setVolume(getVolume(SoundType::Music));
	for (const auto& m : m_soundEffects) m->setVolume(getVolume(m->getSoundType()));
}
