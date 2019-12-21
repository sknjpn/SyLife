#pragma once

#include "Field.h"
#include "Assets.h"

class World
{
	String		m_name;
	FilePath	m_filePath;
	Field		m_field;

	static unique_ptr<World>	g_instance;

	void	load();
	void	make();

public:
	static void Make();
	static void Load(const FilePath& filepath);
	static const unique_ptr<World>& GetInstance() { return g_instance; }

	const Field& getField() const { return m_field; }
	Field& getField() { return m_field; }

	void	setName(const String& name) { m_name = name; }
	void	setFilePath(const FilePath& filePath) { m_filePath = filePath; }

	FilePath	getSaveDirectory() const { return m_filePath; }
	FilePath	getAssetsSaveDirectory() const { return m_filePath + U"resources/"; }
	FilePath	getFieldSaveDirectory() const { return m_filePath + U"field/"; }

	void	update();

	void	save();
};
