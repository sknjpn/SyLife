#pragma once

#include "PartConfig.h"
#include "BodyModel.h"

class BodyConfig
	: public PartConfig
{
public:
	shared_ptr<BodyModel>	m_model;

public:
	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

