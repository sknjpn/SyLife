#include "MoleculeManager.h"
#include "Molecule.h"
#include "FieldManager.h"

unique_ptr<MoleculeManager> g_moleculeManager;

MoleculeManager::MoleculeManager()
{
	m_molecules.reserve(10000);
}


MoleculeManager::~MoleculeManager()
{
}

const shared_ptr<Molecule>& MoleculeManager::AddMolecule()
{
	const auto& m = m_molecules.emplace_back(make_shared<Molecule>());

	g_fieldManager->m_rigidbodies.emplace_back(m);

	return m;
}

void MoleculeManager::Update()
{
	m_molecules.erase(remove_if(m_molecules.begin(), m_molecules.end(), [](const auto& m) { return m->m_destroyFlag; }), m_molecules.end());
}
