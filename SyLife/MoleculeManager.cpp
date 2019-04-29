#include "MoleculeManager.h"

unique_ptr<MoleculeManager> g_moleculeManager;

MoleculeManager::MoleculeManager()
{
}


MoleculeManager::~MoleculeManager()
{
}

const shared_ptr<Molecule>& MoleculeManager::AddMolecule()
{
	return m_molecules.emplace_back(make_shared<Molecule>());
}
