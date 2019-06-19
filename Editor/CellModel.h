#pragma once

#include "Model.h"

#include "BodyModel.h"
#include "EquipmentModel.h"
#include "ModuleModel.h"

class CellModel
	: public Model
{
public:
	string	m_name;
	shared_ptr<BodyModel>	m_body;
	vector<shared_ptr<EquipmentModel>>	m_equipments;
	vector<shared_ptr<ModuleModel>>		m_modules;

public:
	ptree	ToJSON() const override;
	void	FromJSON(const ptree& pt) override;

	string	GetFilepath() const override { return "assets/part/cell/" + GetFilename(); }
};