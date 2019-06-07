#pragma once

#include "Indexer.h"
#include "Cell.h"

class CellManager
{
public:
	Indexer<Cell>	m_indexer;

public:
	const shared_ptr<Cell>&	AddCell();

	vector<shared_ptr<Cell>>&	GetCells() { return m_indexer.m_cloud.m_particles; }
	const vector<shared_ptr<Cell>>&	GetCells() const { return m_indexer.m_cloud.m_particles; }

	void	Update();
};

extern unique_ptr<CellManager>	g_cellManagerPtr;