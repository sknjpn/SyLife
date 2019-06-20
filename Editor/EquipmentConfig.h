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
	EquipmentConfig(const shared_ptr<EquipmentModel>& model, const s3d::Vec2& position, double rotation)
		: m_model(model)
		, m_position(position)
		, m_rotation(rotation)
	{}

	ptree	ToJSON() const override;
	void	FromJSON(const ptree& pt) override;
};

