#pragma once

#include "PartModel.h"

class EquipmentModel
	: public PartModel
{
public:
	ptree	ToJSON() const override;
	void	FromJSON(const ptree& pt) override;

	string	GetFilepath() const override { return "assets/part/equipment/" + GetFilename(); }
};
