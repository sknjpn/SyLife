#pragma once

#include "Part.h"

class EquipmentModel
	: public PartModel
{
public:
	string	GetFilepath() const override { return "assets/part/" + GetFilename(); }

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class EquipmentConfig
	: public PartConfig
{
public:
	shared_ptr<EquipmentModel>	m_model;

public:
	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class EquipmentState
	: public PartState
{
public:
};

