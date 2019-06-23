#pragma once

#include "Indexer.h"
#include "Molecule.h"

class MoleculeManager
{
public:
	Indexer<Molecule>	m_indexer;

public:
	int NumMolecule(const shared_ptr<MoleculeModel>& model);

	const shared_ptr<Molecule>&		AddMolecule(const shared_ptr<MoleculeModel>& model);
	const shared_ptr<Molecule>&		AddMolecule(const shared_ptr<MoleculeModel>& model, const Vector2D& position);

	void	AddMoleculesRandom(const shared_ptr<MoleculeModel>& model, size_t size);

	vector<shared_ptr<Molecule>>&	GetMolecules() { return m_indexer.m_cloud.m_particles; }
	const vector<shared_ptr<Molecule>>&	GetMolecules() const { return m_indexer.m_cloud.m_particles; }

	void	Update();
	void	Draw();
};

extern unique_ptr<MoleculeManager> g_moleculeManagerPtr;