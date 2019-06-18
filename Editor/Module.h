#pragma once

#include "Part.h"

class ModuleModel
	: public PartModel
{
public:
	ptree	ToJSON() const override;
	void	FromJSON(const ptree& pt) const override;

	string	GetFilepath() const override { return "assets/part/module/" + GetFilename(); }
};
