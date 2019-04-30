#include "Storage.h"
#include "MoleculeManager.h"

Storage::Storage()
{

}

Storage::~Storage()
{

}

void Storage::AddMolecule(const shared_ptr<Molecule::Model>& model)
{
	for (auto it = m_molecules.begin(); it != m_molecules.end(); ++it)
	{
		if ((*it).first == model)
		{
			++(*it).second;

			return;
		}
	}

	m_molecules.emplace_back(model, 1);
}

int Storage::NumMolecule(const shared_ptr<Molecule::Model>& model) const
{
	for (auto it = m_molecules.begin(); it != m_molecules.end(); ++it)
	{
		if ((*it).first == model) return (*it).second;
	}

	return 0;
}

int Storage::NumMolecule(const string& name) const
{
	return NumMolecule(g_moleculeManagerPtr->GetModel(name));
}

void Storage::PullMolecule(const shared_ptr<Molecule::Model>& model)
{
	for (auto it = m_molecules.begin(); it != m_molecules.end(); ++it)
	{
		if ((*it).first == model)
		{
			if (--(*it).second <= 0)
			{
				m_molecules.erase(it);
				
			}

			return;
		}
	}
}

void Storage::PullMolecule(const shared_ptr<Molecule::Model>& model, int size)
{
	for (auto it = m_molecules.begin(); it != m_molecules.end(); ++it)
	{
		if ((*it).first == model)
		{
			if (((*it).second -= size) <= 0)
			{
				m_molecules.erase(it);

			}

			return;
		}
	}
}
