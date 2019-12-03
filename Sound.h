#pragma once

enum struct SoundType
{
	Music,
	UI,
	Environment,
};

class Sound
	: public Audio
{
	FilePath	m_filePath;
	SoundType	m_soundType;

public:
	Sound(const FilePath& filePath, SoundType soundType)
		: Audio(filePath)
		, m_filePath(filePath)
		, m_soundType(soundType)
	{}

	const FilePath& getFilePath() const { return m_filePath; }
	SoundType	getSoundType() const { return m_soundType; }
};
