#pragma once

#include "Model.h"

#include "BodyConfig.h"
#include "EquipmentConfig.h"
#include "ModuleConfig.h"

class CellModel
	: public Model
{
public:
	shared_ptr<BodyConfig>				m_body;
	vector<shared_ptr<EquipmentConfig>>	m_equipments;
	vector<shared_ptr<ModuleConfig>>	m_modules;

public:
	CellModel()
		: m_body(make_shared<BodyConfig>(nullptr))
	{}

	void	FromJSON(const ptree& pt) override;

	string	GetFilepath() const override { return "assets/cell/" + GetFilename(); }
};