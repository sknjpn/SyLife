#pragma once

class Molecule;

class MoleculeManager
{
public:
	vector<shared_ptr<Molecule>>	m_molecules;

public:
	MoleculeManager();
	~MoleculeManager();

	const shared_ptr<Molecule>&	AddMolecule();

	void	Update();
};

extern unique_ptr<MoleculeManager> g_moleculeManager;