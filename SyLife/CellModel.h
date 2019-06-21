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

	// ‹ßŽ—‰~
	double	m_mass;
	double	m_radius;
	double	m_inertia;

public:
	CellModel()
		: m_body(make_shared<BodyConfig>())
	{}

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }

	string	GetFilepath() const override { return "assets/cell/" + GetFilename(); }

	void	CalculateDisk();
};