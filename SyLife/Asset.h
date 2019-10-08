#pragma once

#include "Model.h"

// AssetManagerに登録可能なModel
// JSONファイルに対応している
class Asset
	: public Model
{
	string	m_name;

public:
	Asset(const string& name)
		: m_name(name)
	{}

	// Set
	void	setName(const string& name);

	// Get
	const string& getName() const { return m_name; }

	// Path
	virtual string	getFilename() const;
	virtual string	getFilepath() const { return "assets/model/" + getFilename(); }

	// JSON
	void	load_this(const ptree& pt);
	 void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	 void	save(ptree& pt) const override{ save_this(pt); }
};
