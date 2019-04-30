#pragma once

#include "Molecule.h"

class Storage
{
	vector<pair<shared_ptr<Molecule::Model>, int>>	m_molecules;

public:
	Storage();
	~Storage();
};

