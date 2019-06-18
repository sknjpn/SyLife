#pragma once

#include "PartModel.h"

class EquipmentModel
	: public PartModel
{
public:
	void	FromJSON(const ptree& pt) override;

	string	GetFilepath() const override { return "assets/part/equipment/" + GetFilename(); }
};
