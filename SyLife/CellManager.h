#pragma once

#include "Indexer.h"

class Cell;

class CellManager
{
public:
	vector<shared_ptr<Cell>>	m_cells;
	Indexer<Cell>	m_indexer;

public:
	CellManager();
	~CellManager();

	const shared_ptr<Cell>&	AddCell();

	void	Update();
};

extern unique_ptr<CellManager>	g_cellManagerPtr;