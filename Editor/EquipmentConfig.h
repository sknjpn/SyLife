#pragma once

#include "Model.h"
#include "EquipmentModel.h"

class EquipmentConfig
	: public Model
{
public:
	shared_ptr<EquipmentModel>	m_model;
	s3d::Vec2	m_position;
	double		m_rotation;

public:
	EquipmentConfig(const ptree& pt) { FromJSON(pt); }

	ptree	ToJSON() const override;
	void	FromJSON(const ptree& pt) override;
};

