#include "CellManager.h"

unique_ptr<CellManager>	g_cellManager;

CellManager::CellManager()
{
}


CellManager::~CellManager()
{
}

const shared_ptr<Cell>& CellManager::AddCell()
{
	return m_cells.emplace_back();
}
