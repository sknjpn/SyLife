#pragma once

#include "PartModel.h"

class BodyModel
	: public PartModel
{
public:
	ptree	AddToJSON(ptree pt) const;
	void	SetFromJSON(const ptree& pt);

	string	GetFilepath() const override { return "assets/part/body/" + GetFilename(); }

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	Save() const override { return AddToJSON(ptree()); }
};
