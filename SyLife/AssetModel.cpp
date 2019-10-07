#include "AssetModel.h"

void AssetModel::SetName(const string& name)
{
	// 変化がなければ何もしない
	if (name == m_name) return;

	// ファイルの削除
	{
		const boost::filesystem::path path(GetFilepath());

		boost::filesystem::remove(path);
	}

	// nameのセット
	m_name = name;

	// 新規ファイルの作成
	{
		ptree pt;

		save(pt);

		write_json(GetFilepath(), pt);
	}
}

string AssetModel::GetFilename() const
{
	auto filename = m_name + ".json";
	auto f = [](unsigned char c) { return char(c == 0x20 ? 0x5f : tolower(c)); };

	transform(filename.begin(), filename.end(), filename.begin(), f);

	return filename;
}

void AssetModel::load_this(const ptree& pt)
{
	m_name = pt.get<string>("name");
}

void AssetModel::save_this(ptree& pt) const
{
	// name
	pt.put("name", m_name);

	// type
	pt.put("type", "Model");
}
