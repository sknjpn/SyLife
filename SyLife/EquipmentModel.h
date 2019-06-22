#pragma once

#include "PartModel.h"

class EquipmentModel
	: public PartModel
{
public:
	string	GetFilepath() const override { return "assets/part/" + GetFilename(); }

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};
