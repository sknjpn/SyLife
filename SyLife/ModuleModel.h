#pragma once

#include "PartModel.h"

class ModuleModel
	: public PartModel
{
public:
	void	FromJSON(const ptree& pt) override;

	string	GetFilepath() const override { return "assets/part/module/" + GetFilename(); }
};
