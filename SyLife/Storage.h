#pragma once

#include "Model.h"
#include "AssetManager.h"
#include "Molecule.h"

class MoleculeModel;

class Storage
	: public Model
{
public:
	vector<pair<shared_ptr<MoleculeModel>, unsigned int>>	m_molecules;

public:
	void	AddMoleculeState(const shared_ptr<MoleculeModel>& model, unsigned int size = 1);
	int		NumMolecule(const shared_ptr<MoleculeModel>& model) const;
	int		NumMolecule(const string& name) const;
	bool	HasMolecule(const Storage& storage);
	void	PullMolecule(const shared_ptr<MoleculeModel>& model, unsigned int size = 1);
	void	PullMolecule(const Storage& storage);
	void	Clear() { m_molecules.clear(); }
	void	AddStorage(const Storage& storage);

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
