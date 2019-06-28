#pragma once

#include "Molecule.h"

class Storage
{
public:
	vector<pair<shared_ptr<MoleculeModel>, int>>	m_molecules;

public:
	void	AddMoleculeState(const shared_ptr<MoleculeModel>& model);
	void	AddMoleculeState(const shared_ptr<MoleculeModel>& model, int size);
	int		NumMolecule(const shared_ptr<MoleculeModel>& model) const;
	int		NumMolecule(const string& name) const;
	void	PullMolecule(const shared_ptr<MoleculeModel>& model);
	void	PullMolecule(const shared_ptr<MoleculeModel>& model, int size);

	void	Clear() { m_molecules.clear(); }
};

