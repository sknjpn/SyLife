#pragma once

#include "Model.h"
#include "ModuleModel.h"

class ModuleConfig
	: public Model
{
public:
	shared_ptr<ModuleModel>	m_model;
	s3d::Vec2	m_position;
	double		m_rotation;

public:
	ModuleConfig(const ptree& pt) { FromJSON(pt); }

	ptree	ToJSON() const override;
	void	FromJSON(const ptree& pt) override;
};

