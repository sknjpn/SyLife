#pragma once

#include "Part.h"

class EquipmentModel
	: public PartModel
{
public:
	ptree	ToJSON() const override;
	void	Load(const string& filepath) override;
	void	Save() const override;
};
