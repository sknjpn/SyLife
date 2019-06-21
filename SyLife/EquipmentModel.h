#pragma once

#include "PartModel.h"

class EquipmentModel
	: public PartModel
{
public:
	void	SetFromJSON(const ptree& pt);

	string	GetFilepath() const override { return "assets/part/equipment/" + GetFilename(); }

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};
