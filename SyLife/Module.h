#pragma once

#include "Part.h"

class ModuleModel
	: public PartModel
{
public:
	void	SetFromJSON(const ptree& pt);

	string	GetFilepath() const override { return "assets/part/" + GetFilename(); }

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class ModuleConfig
	: public PartConfig
{
public:
	shared_ptr<ModuleModel>	m_model;

public:
	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class ModuleState
	: public PartState
{
public:
};

