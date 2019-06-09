#pragma once

#include "Part.h"

class ModuleModel
	: public PartModel
{
public:
	ptree	ToJSON() const override;
	void	Load(const string& filepath) override;

	string	GetFilepath() const override { return "asset/part/module/" + GetFilename(); }
};
