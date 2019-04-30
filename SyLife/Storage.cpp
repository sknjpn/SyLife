#include "Storage.h"


Storage::Storage()
{
}


Storage::~Storage()
{
}

void Storage::AddMolecule(const shared_ptr<Molecule>& molecule)
{
	for (auto it = m_molecules.begin(); it != m_molecules.end(); ++it)
	{
		if ((*it).first == molecule->m_model)
		{
			++(*it).second;

			return;
		}
	}

	m_molecules.emplace_back(molecule->m_model, 1);
}
