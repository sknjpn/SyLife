#pragma once

#include "Part.h"

class BodyModel
	: public PartModel
{
public:
	ptree	ToJSON() const override;
};
