#pragma once

#include "Model.h"

#include "BodyConfig.h"
#include "EquipmentConfig.h"
#include "ModuleConfig.h"

#include "PartConfig.h"

class CellModel
	: public Model
{
public:
	shared_ptr<BodyConfig>	m_body;
	vector<shared_ptr<EquipmentConfig>>	m_equipments;
	vector<shared_ptr<ModuleConfig>>	m_modules;

	vector<shared_ptr<PartConfig>>		m_parts;

public:
	CellModel()
		: m_body(make_shared<BodyConfig>())
	{}

	ptree	AddToJSON(ptree pt) const;
	void	SetFromJSON(const ptree& pt);

	string	GetFilepath() const override { return "assets/cell/" + GetFilename(); }

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	Save() const override { return AddToJSON(ptree()); }
};