#pragma once

#include "Part.h"

class EquipmentModel
	: public PartModel
{
public:
	ptree	ToJSON() const override;
	void	Load(const string& path) const override;
	void	Save() const override;
};
