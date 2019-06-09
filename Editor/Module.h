#pragma once

#include "Part.h"

class ModuleModel
	: public PartModel
{
public:
	ptree	ToJSON() const override;
};
