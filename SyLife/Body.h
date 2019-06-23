#pragma once

#include "Part.h"

class BodyModel
	: public PartModel
{
public:
	string	GetFilepath() const override { return "assets/part/" + GetFilename(); }

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class BodyConfig
	: public PartConfig
{
public:
	shared_ptr<BodyModel>	m_model;

public:
	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class BodyState
	: public PartState
{
public:
};
