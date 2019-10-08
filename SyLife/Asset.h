#pragma once

#include "Model.h"

// AssetManagerに登録可能なModel
// JSONファイルに対応している
class Asset
	: public Model
{
	string	m_name;
	string	m_filepath;

public:
	// Set
	void	setName(const string& name) { m_name = m_filepath; }
	void	setFilepath(const string& filepath);

	// Get
	const string& getName() const { return m_name; }
	const string& getFilepath() const { return m_filepath; }

	// JSON
	void	load_this(const ptree& pt);
	 void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	 void	save(ptree& pt) const override{ save_this(pt); }
};
