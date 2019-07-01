#pragma once

#include "Indexer.h"

class MoleculeState;
class MoleculeModel;

class MoleculeManager
{
	Indexer<MoleculeState>	m_indexer;

public:
	int NumMolecule(const shared_ptr<MoleculeModel>& model);

	const shared_ptr<MoleculeState>&	AddMoleculeState(const shared_ptr<MoleculeModel>& model);
	const shared_ptr<MoleculeState>&	AddMoleculeState(const shared_ptr<MoleculeModel>& model, const s3d::Vec2& position);

	void	AddMoleculesRandom(const shared_ptr<MoleculeModel>& model, size_t size);

	// Get
	vector<shared_ptr<MoleculeState>>&	GetMoleculeStates() { return m_indexer.m_cloud.m_particles; }
	const vector<shared_ptr<MoleculeState>>&	GetMoleculeStates() const { return m_indexer.m_cloud.m_particles; }
	Indexer<MoleculeState>&	GetIndexer();
	const Indexer<MoleculeState>&	GetIndexer() const;

	void	Update();
	void	Draw();
};

extern unique_ptr<MoleculeManager> g_moleculeManagerPtr;