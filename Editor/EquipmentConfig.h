#pragma once

#include "EquipmentModel.h"

class EquipmentConfig
{
public:
	shared_ptr<EquipmentModel>	m_model;
	s3d::Vec2	m_position;
	double		m_rotation;

public:
	EquipmentConfig(const shared_ptr<EquipmentModel>& model)
		: m_model(model)
	{}
};

