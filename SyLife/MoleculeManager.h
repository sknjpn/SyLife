#pragma once

#include "Molecule.h"

class MoleculeManager
{
public:
	vector<shared_ptr<Molecule>>		m_molecules;
	vector<shared_ptr<Molecule::Model>>	m_models;

public:
	MoleculeManager();
	~MoleculeManager();

	const shared_ptr<Molecule>&			AddMolecule(const shared_ptr<Molecule::Model>& model);
	const shared_ptr<Molecule::Model>&	AddModel();

	void	AddMoleculesRandom(const shared_ptr<Molecule::Model>& model, size_t size);

	void	Update();
};

extern unique_ptr<MoleculeManager> g_moleculeManager;