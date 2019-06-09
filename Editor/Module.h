#pragma once

#include "Part.h"

class ModuleModel
	: public PartModel
{
public:
	ptree	ToJSON() const override;
	void	Load(const string& path) const override;
	void	Save() const override;
};
