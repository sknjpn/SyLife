#pragma once

#include "Rigidbody.h"
#include "Storage.h"
#include "Molecule.h"

#include "CellModel.h"
#include "MoleculeModel.h"

class Cell : public Rigidbody
{
public:
	double	m_deathTimer;
	Storage	m_storage;
	Storage	m_molecules;

	shared_ptr<CellModel>	m_model;

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

