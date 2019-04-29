#pragma once

class Cell;

class CellManager
{
public:
	vector<shared_ptr<Cell>>	g_cells;

public:
	CellManager();
	~CellManager();
};

extern unique_ptr<CellManager>	g_cellManager;