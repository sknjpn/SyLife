#pragma once

#include "ModuleModel.h"

class ModuleConfig
{
public:
	shared_ptr<ModuleModel>	m_model;
	s3d::Vec2	m_position;
	double		m_rotation;

public:
	ModuleConfig(const shared_ptr<ModuleModel>& model)
		: m_model(model)
	{}
};

