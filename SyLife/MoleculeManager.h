#pragma once

#include "Indexer.h"

class MoleculeState;
class MoleculeModel;

class MoleculeManager
{
public:
	Indexer<MoleculeState>	m_indexer;

public:
	int NumMolecule(const shared_ptr<MoleculeModel>& model);

	const shared_ptr<MoleculeState>&	AddMoleculeState(const shared_ptr<MoleculeModel>& model);
	const shared_ptr<MoleculeState>&	AddMoleculeState(const shared_ptr<MoleculeModel>& model, const Vector2D& position);

	void	AddMoleculesRandom(const shared_ptr<MoleculeModel>& model, size_t size);

	vector<shared_ptr<MoleculeState>>&	GetMoleculeStates() { return m_indexer.m_cloud.m_particles; }
	const vector<shared_ptr<MoleculeState>>&	GetMoleculeStates() const { return m_indexer.m_cloud.m_particles; }

	void	Update();
	void	Draw();
};

extern unique_ptr<MoleculeManager> g_moleculeManagerPtr;