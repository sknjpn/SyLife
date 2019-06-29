#include "Storage.h"
#include "AssetManager.h"

void Storage::AddMoleculeState(const shared_ptr<MoleculeModel>& model)
{
	auto it = find_if(m_molecules.begin(), m_molecules.end(), [&model](const auto& m) { return m.first == model; });

	if (it == m_molecules.end()) m_molecules.emplace_back(model, 1);
	else ++(*it).second;
}

void Storage::AddMoleculeState(const shared_ptr<MoleculeModel>& model, int size)
{
	auto it = find_if(m_molecules.begin(), m_molecules.end(), [&model](const auto& m) { return m.first == model; });

	if (it == m_molecules.end()) m_molecules.emplace_back(model, 1);
	else (*it).second += size;
}

int Storage::NumMolecule(const shared_ptr<MoleculeModel>& model) const
{
	auto it = find_if(m_molecules.begin(), m_molecules.end(), [&model](const auto& m) { return m.first == model; });

	if (it == m_molecules.end()) return 0;
	else return (*it).second;
}

int Storage::NumMolecule(const string& name) const
{
	return NumMolecule(g_assetManagerPtr->GetModel<MoleculeModel>(name));
}

void Storage::PullMolecule(const shared_ptr<MoleculeModel>& model)
{
	auto it = find_if(m_molecules.begin(), m_molecules.end(), [&model](const auto& m) { return m.first == model; });

	if (it == m_molecules.end()) throw new exception;
	else
	{
		if ((--(*it).second) < 0) throw new exception;
		else if ((*it).second == 0) m_molecules.erase(it);
	}
}

void Storage::PullMolecule(const shared_ptr<MoleculeModel>& model, int size)
{
	auto it = find_if(m_molecules.begin(), m_molecules.end(), [&model](const auto& m) { return m.first == model; });

	if (it == m_molecules.end()) throw new exception;
	else
	{
		if (((*it).second -= size) < 0) throw new exception;
		else if ((*it).second == 0) m_molecules.erase(it);
	}
}

void Storage::AddStorage(const Storage& storage)
{
	for (const auto& m : storage.m_molecules)
		AddMoleculeState(m.first, m.second);
}
