#pragma once

#include "Indexer.h"
#include "FieldManager.h"

class CellState;
class CellModel;

class CellManager
{
	Indexer<CellState>	m_indexer;

public:

	// Get
	Indexer<CellState>&	GetIndexer() { return m_indexer; }
	const Indexer<CellState>&	GetIndexer() const { return m_indexer; }
	vector<shared_ptr<CellState>>&	GetCellStates() { return m_indexer.m_cloud.m_particles; }
	const vector<shared_ptr<CellState>>&	GetCellStates() const { return m_indexer.m_cloud.m_particles; }

	const shared_ptr<CellState>&	AddCellState(const shared_ptr<CellModel>& model);

	void	Update();
	void	Draw();
};

extern unique_ptr<CellManager>	g_cellManagerPtr;