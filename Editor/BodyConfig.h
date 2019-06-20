#pragma once

#include "Model.h"
#include "BodyModel.h"

class BodyConfig
	: public Model
{
public:
	shared_ptr<BodyModel>	m_model;

public:
	BodyConfig(const shared_ptr<BodyModel>& model)
		: m_model(model)
	{}

	ptree	ToJSON() const override;
	void	FromJSON(const ptree& pt) override;
};

