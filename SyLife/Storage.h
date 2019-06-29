#pragma once

#include "Model.h"
#include "Molecule.h"
#include "AssetManager.h"

class MoleculeModel;

class Storage
	: public Model
{
public:
	vector<pair<shared_ptr<MoleculeModel>, unsigned int>>	m_molecules;

public:
	Storage	operator +(const Storage& s) const { return Storage(*this) += s; }
	bool	operator <=(const Storage& s) const 
	{
		for (const auto& m : s.m_molecules)
			if (Num(m.first) < m.second) return false;

		return true;
	}

	Storage& operator +=(const Storage& s) noexcept
	{
		for (const auto& m : s.m_molecules)
			Add(m.first, m.second);

		return *this;
	}

	Storage& operator +=(const Storage& s) noexcept
	{
		for (const auto& m : s.m_molecules)
			Pull(m.first, m.second);

		return *this;
	}

	void	Add(const shared_ptr<MoleculeModel>& model, unsigned int size = 1)
	{
		auto it = find_if(m_molecules.begin(), m_molecules.end(), [&model](const auto& m) { return m.first == model; });

		if (it == m_molecules.end()) m_molecules.emplace_back(model, 1);
		else (*it).second += size;
	}
	
	int		Num(const shared_ptr<MoleculeModel>& model) const
	{
		auto it = find_if(m_molecules.begin(), m_molecules.end(), [&model](const auto& m) { return m.first == model; });

		if (it == m_molecules.end()) return 0;
		else return (*it).second;
	}

	void	Pull(const shared_ptr<MoleculeModel>& model, unsigned int size = 1)
	{
		auto it = find_if(m_molecules.begin(), m_molecules.end(), [&model](const auto& m) { return m.first == model; });

		if (it == m_molecules.end()) throw new exception;
		else
		{
			if (((*it).second -= size) < 0) throw new exception;
			else if ((*it).second == 0) m_molecules.erase(it);
		}
	}

	void	Clear() { m_molecules.clear(); }

	// JSON
	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

inline void Storage::SetFromJSON(const ptree& pt)
{
	// molecules
	for (auto m : pt.get_child("molecules"))
	{
		const auto& model = g_assetManagerPtr->GetModel<MoleculeModel>(m.second.get<string>("model"));

		m_molecules.emplace_back(model, m.second.get<int>("size"));
	}

	Model::SetFromJSON(pt);
}
