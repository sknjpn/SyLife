#pragma once

#include "Molecule.h"

class Storage
{
public:
	vector<pair<shared_ptr<Molecule::Model>, int>>	m_molecules;

public:
	Storage();
	~Storage();

	void	AddMolecule(const shared_ptr<Molecule>& molecule);
	int		NumMolecule(const shared_ptr<Molecule::Model>& model) const;
	void	PullMolecule(const shared_ptr<Molecule::Model>& model);
	void	PullMolecule(const shared_ptr<Molecule::Model>& model, int size);
};

