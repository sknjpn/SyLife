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

	int NumMolecule(const shared_ptr<Molecule::Model>& model);
	const shared_ptr<Molecule>&			AddMolecule(const shared_ptr<Molecule::Model>& model);
	const shared_ptr<Molecule>&			AddMolecule(const shared_ptr<Molecule::Model>& model, const Vector2D& position);
	const shared_ptr<Molecule::Model>&	AddModel();

	const shared_ptr<Molecule::Model>&	GetModel(const string& name) const;

	void	AddMoleculesRandom(const shared_ptr<Molecule::Model>& model, size_t size);


	void	Update();
};

extern unique_ptr<MoleculeManager> g_moleculeManager;