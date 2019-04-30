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
};

