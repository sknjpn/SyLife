#pragma once

#include "Part.h"

class BodyModel
	: public PartModel
{
public:
	ptree	ToJSON() const override;
	void	FromJSON(const ptree& pt) override;

	string	GetFilepath() const override { return "assets/part/body/" + GetFilename(); }
};
