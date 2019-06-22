#pragma once

#include "Rigidbody.h"
#include "Storage.h"
#include "Molecule.h"

#include "CellModel.h"
#include "MoleculeModel.h"

#include "BodyState.h"
#include "EquipmentState.h"
#include "ModuleState.h"

#include "PartState.h"

class Cell 
	: public Rigidbody
{
public:
	double	m_deathTimer;
	Storage	m_storage;
	Storage	m_molecules;

	shared_ptr<CellModel>	m_model;

	shared_ptr<BodyState>	m_body;
	vector<shared_ptr<EquipmentState>>	m_equipments;
	vector<shared_ptr<ModuleState>>	m_modules;

	vector<shared_ptr<PartState>>		m_parts;

public:
	Cell(const shared_ptr<CellModel>& model)
		: m_model(model)
		, m_deathTimer(10.0)
	{
		m_mass = m_model->m_mass;
		m_radius = m_model->m_radius;
		m_inertia = m_model->m_inertia;
	}

	void	Update();
	void	Draw();

	// Field‚©‚çStorage‚Éó‚¯æ‚é
	void	TakeMolecule(const shared_ptr<Molecule>& molecule);

	// Storage‚©‚çField‚É“f‚«o‚·
	void	ExpireMolecule(const shared_ptr<MoleculeModel>& model);
	void	ExpireMolecule(const shared_ptr<MoleculeModel>& model, int size);
};

