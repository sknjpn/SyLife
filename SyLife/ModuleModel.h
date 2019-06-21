#pragma once

#include "PartModel.h"

class ModuleModel
	: public PartModel
{
public:
	void	SetFromJSON(const ptree& pt);

	string	GetFilepath() const override { return "assets/part/module/" + GetFilename(); }

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};
