#pragma once

#include "Viewer.h"

class Model
	: public enable_shared_from_this<Model>
{
	string	m_name;

public:
	virtual ~Model() = default;

	virtual void	MakeViewer() {}

	// Set
	void	SetName(const string& name)
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

			Save(pt);

			write_json(GetFilepath(), pt);
		}
	}

	// Get
	const string&	GetName() const { return m_name; }

	// Path
	virtual string	GetFilename() const
	{
		auto filename = m_name + ".json";
		auto f = [](unsigned char c) { return char(c == 0x20 ? 0x5f : tolower(c)); };

		transform(filename.begin(), filename.end(), filename.begin(), f);

		return filename;
	}
	virtual string	GetFilepath() const
	{
		return "assets/model/" + GetFilename();
	}

	// JSON
	void	Load_this(const ptree& pt)
	{
		m_name = pt.get<string>("name");
	}
	virtual void	Load(const ptree& pt) { Load_this(pt); }
	void	Save_this(ptree& pt) const {
		// name
		pt.put("name", m_name);

		// type
		pt.put("type", "Model");
	}
	virtual void	Save(ptree& pt) const { Save_this(pt); }
};
