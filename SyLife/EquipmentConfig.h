#pragma once

#include "PartConfig.h"
#include "EquipmentModel.h"

class EquipmentConfig
	: public PartConfig
{
public:
	shared_ptr<EquipmentModel>	m_model;

public:
	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

