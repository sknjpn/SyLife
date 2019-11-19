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

	m_name = json[U"name"].getString();
}

void Asset::save(JSONWriter& json) const
{
	Model::save(json);

	// name
	json.key(U"name").write(m_name);

	// type
	json.key(U"type").write(U"Asset");
}
