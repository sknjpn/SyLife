#include "Asset.h"

void Asset::setFilePath(const String& filepath)
{
	if (filepath == m_filepath || m_filepath == U"")
	{
		m_filepath = filepath;

		return;
	}

	// ファイルの削除
	FileSystem::Remove(getFilePath());

	// nameのセット
	m_filepath = filepath;

	// 新規ファイルの作成
	{
		JSONWriter json;

		save(json);

		json.write(getFilePath());
	}
}

void Asset::load(const JSONValue& json)
{
	Model::load(json);

	// m_name = pt.get<String>(U"name");
}

void Asset::save(const JSONWriter& json) const
{
	Model::save(json);

	// name
	//pt.put(U"name", m_name);

	// type
	//pt.put(U"type", "Model");
}
