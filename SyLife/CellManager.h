#pragma once

#include "Indexer.h"

class CellState;
class CellModel;

class CellManager
{
public:
	Indexer<CellState>	m_indexer;

public:
	const shared_ptr<CellState>&	AddCellState(const shared_ptr<CellModel>& model);

	vector<shared_ptr<CellState>>&	GetCellStates() { return m_indexer.m_cloud.m_particles; }
	const vector<shared_ptr<CellState>>&	GetCellStates() const { return m_indexer.m_cloud.m_particles; }

	void	Update();
	void	Draw();
};

extern unique_ptr<CellManager>	g_cellManagerPtr;