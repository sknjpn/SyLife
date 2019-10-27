#include "Asset.h"

void Asset::setFilepath(const string& filepath)
{
	if (filepath == m_filepath || m_filepath == "")
	{
		m_filepath = filepath; 
		
		return;
	}

	// ファイルの削除
	{
		const boost::filesystem::path path(getFilepath());

		boost::filesystem::remove(path);
	}

	// nameのセット
	m_filepath = filepath;

	// 新規ファイルの作成
	{
		ptree pt;

		save(pt);

		write_json(getFilepath(), pt);
	}
}

void Asset::load(const ptree& pt)
{
	Model::load(pt);

	m_name = pt.get<string>("name");
}

void Asset::save(ptree& pt) const
{
	Model::save(pt);

	// name
	pt.put("name", m_name);

	// type
	pt.put("type", "Model");
}
