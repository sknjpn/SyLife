#pragma once

#include "Indexer.h"
#include "Molecule.h"

class MoleculeManager
{
public:
	vector<shared_ptr<MoleculeModel>>	m_models;
	Indexer<Molecule>	m_indexer;

public:
	MoleculeManager();
	~MoleculeManager();

	int NumMolecule(const shared_ptr<MoleculeModel>& model);
	const shared_ptr<Molecule>&			AddMolecule(const shared_ptr<MoleculeModel>& model);
	const shared_ptr<Molecule>&			AddMolecule(const shared_ptr<MoleculeModel>& model, const Vector2D& position);
	const shared_ptr<MoleculeModel>&	AddModel();

	const shared_ptr<MoleculeModel>&	GetModel(const string& name) const;

	void	AddMoleculesRandom(const shared_ptr<MoleculeModel>& model, size_t size);

	vector<shared_ptr<Molecule>>&	GetMolecules() { return m_indexer.m_cloud.m_particles; }
	const vector<shared_ptr<Molecule>>&	GetMolecules() const { return m_indexer.m_cloud.m_particles; }

	void	Init();
	void	Update();

	void	ImportModelFromFile(const string& filePath);
};

extern unique_ptr<MoleculeManager> g_moleculeManagerPtr;