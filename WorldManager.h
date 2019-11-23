#pragma once

class WorldManager
{
	int			m_updateSpeed;
	String		m_name;
	FilePath	m_filePath;

public:
	void	save();
	void	load(const FilePath& filepath);

	int		getUpdateSpeed() const { return m_updateSpeed; }
	void	setUpdateSpeed(int speed) { m_updateSpeed = speed; }

	const String& getName() const { return m_name; }
	const FilePath& getFilePath() const { return m_filePath; }

	void	setName(const String& name) { m_name = name; }

	void	update();
};

extern unique_ptr<WorldManager>	g_worldManagerPtr;
