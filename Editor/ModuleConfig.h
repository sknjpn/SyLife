#pragma once

#include "PartConfig.h"
#include "ModuleModel.h"

class ModuleConfig
	: public PartConfig
{
public:
	shared_ptr<ModuleModel>	m_model;

public:
	ptree	AddToJSON(ptree pt) const;
	void	SetFromJSON(const ptree& pt);

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	Save() const override { return AddToJSON(ptree()); }
};

