#include "Asset.h"

void Asset::setFilepath(const string& filepath)
{
	if (filepath == m_filepath || m_filepath == "") return;

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

string Asset::getFilename() const
{
	auto filename = m_name + ".json";
	auto f = [](unsigned char c) { return char(c == 0x20 ? 0x5f : tolower(c)); };

	transform(filename.begin(), filename.end(), filename.begin(), f);

	return filename;
}

void Asset::load_this(const ptree& pt)
{
	m_name = pt.get<string>("name");
}

void Asset::save_this(ptree& pt) const
{
	// name
	pt.put("name", m_name);

	// type
	pt.put("type", "Model");
}
