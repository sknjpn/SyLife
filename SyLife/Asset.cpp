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
		ptree pt;

		save(pt);

		write_json(Unicode::Narrow(getFilePath()), pt);
	}
}

void Asset::load(const JSONValue& json)
{
	Model::load(json);

	// m_name = pt.get<String>(U"name");
}

void Asset::save(ptree& pt) const
{
	Model::save(pt);

	// name
	//pt.put(U"name", m_name);

	// type
	//pt.put(U"type", "Model");
}
