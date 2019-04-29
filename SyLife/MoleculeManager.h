#pragma once

class Molecule;

class MoleculeManager
{
	vector<shared_ptr<Molecule>>	m_molecules;

public:
	MoleculeManager();
	~MoleculeManager();
};

extern unique_ptr<MoleculeManager> g_moleculeManager;