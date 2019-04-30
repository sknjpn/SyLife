#pragma once

#include "Molecule.h"

class Storage
{
public:
	vector<pair<shared_ptr<Molecule::Model>, int>>	m_molecules;

public:
	Storage();
	~Storage();

	void	AddMolecule(const shared_ptr<Molecule::Model>& model);
	int		NumMolecule(const shared_ptr<Molecule::Model>& model) const;
	int		NumMolecule(const string& name) const;
	void	PullMolecule(const shared_ptr<Molecule::Model>& model);
	void	PullMolecule(const shared_ptr<Molecule::Model>& model, int size);
};

