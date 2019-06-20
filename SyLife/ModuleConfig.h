#pragma once

#include "Model.h"
#include "ModuleModel.h"

class ModuleConfig
	: public Model
{
public:
	shared_ptr<ModuleModel>	m_model;
	Vector2D	m_position;
	double		m_rotation;

public:
	ModuleConfig(const ptree& pt) { FromJSON(pt); }
	ModuleConfig(const shared_ptr<ModuleModel>& model, const s3d::Vec2& position, double rotation)
		: m_model(model)
		, m_position(position)
		, m_rotation(rotation)
	{}

	void	FromJSON(const ptree& pt) override;
};

