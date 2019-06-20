#pragma once

#include "PartConfig.h"
#include "BodyModel.h"

class BodyConfig
	: public PartConfig
{
public:
	shared_ptr<BodyModel>	m_model;

public:
	ptree	AddToJSON(ptree pt) const;
	void	SetFromJSON(const ptree& pt);

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	Save() const override { return AddToJSON(ptree()); }
};

