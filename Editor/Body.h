#pragma once

#include "Part.h"

class BodyModel
	: public PartModel
{
public:
	ptree	ToJSON() const override;
	void	Load(const string& path) const override;
	void	Save() const override;
};
