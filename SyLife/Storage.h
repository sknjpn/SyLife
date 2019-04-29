#pragma once

class Molecule;

class Storage
{
	vector<pair<shared_ptr<Molecule>, int>>	m_molecules;

public:
	Storage();
	~Storage();
};

