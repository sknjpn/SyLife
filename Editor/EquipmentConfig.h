#pragma once

#include "PartConfig.h"
#include "EquipmentModel.h"

class EquipmentConfig
	: public PartConfig
{
public:
	shared_ptr<EquipmentModel>	m_model;

public:
	ptree	AddToJSON(ptree pt) const;
	void	SetFromJSON(const ptree& pt);

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	Save() const override { return AddToJSON(ptree()); }
};

