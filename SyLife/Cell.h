#pragma once

#include "Rigidbody.h"
#include "Storage.h"
#include "Molecule.h"

#include "Body.h"
#include "Equipment.h"
#include "Module.h"

class Cell : public Rigidbody
{
public:
	double	m_deathTimer;
	Storage	m_storage;
	Storage	m_molecules;
	shared_ptr<Body>	m_body;
	vector<shared_ptr<Module>>		m_modules;
	vector<shared_ptr<Equipment>>	m_equipments;

public:
	Cell() { m_deathTimer = 10.0; }

	void	RecalculatePhysicalProperty();
	void	Update();
	void	Draw();

	// Field‚©‚çStorage‚Éó‚¯æ‚é
	void	TakeMolecule(const shared_ptr<Molecule>& molecule);

	// Storage‚©‚çField‚É“f‚«o‚·
	void	ExpireMolecule(const shared_ptr<MoleculeModel>& model);
	void	ExpireMolecule(const shared_ptr<MoleculeModel>& model, int size);
};

