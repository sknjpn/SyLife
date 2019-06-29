#pragma once

#include "Model.h"

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
