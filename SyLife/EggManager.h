#pragma once

#include "Indexer.h"

class EggState;
class CellModel;

class EggManager
{
	Indexer<EggState>	m_indexer;

public:
	const shared_ptr<EggState>&	AddEggState(const shared_ptr<CellModel>& cellModel);

	vector<shared_ptr<EggState>>&	GetEggStates() { return m_indexer.m_cloud.m_particles; }
	const vector<shared_ptr<EggState>>&	GetEggStates() const { return m_indexer.m_cloud.m_particles; }

	Indexer<EggState>&	GetIndexer();
	const Indexer<EggState>&	GetIndexer() const;


	void	Update();
	void	Draw();
};

extern unique_ptr<EggManager> g_eggManagerPtr;
