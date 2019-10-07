﻿#pragma once

#include "Model.h"

// AssetManagerに登録可能なModel
// JSONファイルに対応している
class AssetModel
{
	string	m_name;

public:
	AssetModel(const string& name)
		: m_name(name)
	{}

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

			save(pt);

			write_json(GetFilepath(), pt);
		}
	}

	// Get
	const string& GetName() const { return m_name; }

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
	void	load_this(const ptree& pt)
	{
		m_name = pt.get<string>("name");
	}
	virtual void	load(const ptree& pt) { load_this(pt); }
	void	save_this(ptree& pt) const {
		// name
		pt.put("name", m_name);

		// type
		pt.put("type", "Model");
	}
	virtual void	save(ptree& pt) const { save_this(pt); }
};
