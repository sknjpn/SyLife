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
	void	PullMolecule(const shared_ptr<MoleculeModel>& model, unsigned int size = 1);
	void	Clear() { m_molecules.clear(); }
	void	AddStorage(const Storage& storage);

	// JSON
	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const;
	void	Save(ptree& pt) const override { AddToJSON(pt); }
};

void	Storage::SetFromJSON(const ptree& pt)
{
	// molecules
	for (auto m : pt.get_child("molecules"))
	{
		const auto& model = g_assetManagerPtr->GetModel<MoleculeModel>(m.second.get<string>("model"));

		m_molecules.emplace_back(model, m.second.get<int>("size"));
	}

	Model::SetFromJSON(pt);
}

void	Storage::AddToJSON(ptree& pt) const
{
	// molecules
	{
		ptree molecules;

		{
			ptree molecule;

			for (const auto& m : m_molecules)
			{
				molecule.put("model", m.first->GetName());
				molecule.put("size", m.second);

				molecules.push_back(std::make_pair("", molecule));
			}
		}

		pt.add_child("molecules", molecules);
	}

	Model::AddToJSON(pt);

	// type
	pt.put("type", "Storage");
}
