#include "Storage.h"
#include "AssetManager.h"

void Storage::AddMoleculeState(const shared_ptr<MoleculeModel>& model)
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

void Storage::AddMoleculeState(const shared_ptr<MoleculeModel>& model, int size)
{
	for (auto it = m_molecules.begin(); it != m_molecules.end(); ++it)
	{
		if ((*it).first == model)
		{
			(*it).second += size;

			return;
		}
	}

	m_molecules.emplace_back(model, size);
}

int Storage::NumMolecule(const shared_ptr<MoleculeModel>& model) const
{
	for (auto it = m_molecules.begin(); it != m_molecules.end(); ++it)
	{
		if ((*it).first == model) return (*it).second;
	}

	return 0;
}

int Storage::NumMolecule(const string& name) const
{
	return NumMolecule(g_assetManagerPtr->GetModel<MoleculeModel>(name));
}

void Storage::PullMolecule(const shared_ptr<MoleculeModel>& model)
{
	for (auto it = m_molecules.begin(); it != m_molecules.end(); ++it)
	{
		if ((*it).first == model)
		{
			if (--(*it).second <= 0) m_molecules.erase(it);

			return;
		}
	}
}

void Storage::PullMolecule(const shared_ptr<MoleculeModel>& model, int size)
{
	for (auto it = m_molecules.begin(); it != m_molecules.end(); ++it)
	{
		if ((*it).first == model)
		{
			if (((*it).second -= size) <= 0) m_molecules.erase(it);

			return;
		}
	}
}

void Storage::AddStorage(const Storage& storage)
{
	for (const auto& m : storage.m_molecules)
	{
		AddMoleculeState(m.first, m.second);
	}
}
