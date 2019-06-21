#pragma once

#include "PartConfig.h"
#include "ModuleModel.h"

class ModuleConfig
	: public PartConfig
{
public:
	shared_ptr<ModuleModel>	m_model;

public:
	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

