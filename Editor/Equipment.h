#pragma once

#include "Part.h"

class EquipmentModel
	: public PartModel
{
public:
	ptree	ToJSON() const override;
};
