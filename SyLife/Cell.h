#pragma once

#include "Model.h"
#include "Rigidbody.h"

#include "Storage.h"

class PartConfig;
class PartState;
class BodyState;
class EquipmentState;
class ModuleState;

class CellModel
	: public Model
{
public:
	Storage	m_material;
	vector<shared_ptr<PartConfig>>	m_partConfigs;

	// Rigidbody
	double	m_mass;
	double	m_radius;
	double	m_inertia;

public:
	template <typename T>
	vector<shared_ptr<T>>	GetParts() const
	{
		vector<shared_ptr<T>> tModels;

		for (auto it = m_partConfigs.begin(); it != m_partConfigs.end(); ++it)
			if (dynamic_pointer_cast<T>(*it) != nullptr) tModels.emplace_back(dynamic_pointer_cast<T>(*it));

		return tModels;
	}

	void	Draw(double a = 0.5);

	// JSON
	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }

	void	UpdateProperties();
};

class CellState
	: public Rigidbody
{
public:
	double	m_startTimer;
	double	m_deathTimer;
	Storage	m_storage;

	shared_ptr<CellModel>	m_model;

	vector<shared_ptr<PartState>>		m_partStates;

	shared_ptr<BodyState>	m_body;
	vector<shared_ptr<EquipmentState>>	m_equipments;
	vector<shared_ptr<ModuleState>>		m_modules;

public:
	CellState(const shared_ptr<CellModel>& model);

	void	UpdateCell();
	void	Draw();

	void	TakeMolecule(const shared_ptr<MoleculeState>& molecule);
	void	ExpireMolecule(const shared_ptr<MoleculeModel>& model, unsigned int size = 1);
};
