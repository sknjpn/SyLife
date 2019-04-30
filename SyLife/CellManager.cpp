#include "CellManager.h"
#include "Cell.h"
#include "FieldManager.h"

unique_ptr<CellManager>	g_cellManager;

CellManager::CellManager()
{
	m_cells.reserve(10000);
}


CellManager::~CellManager()
{
}

const shared_ptr<Cell>& CellManager::AddCell()
{
	const auto& c = m_cells.emplace_back(make_shared<Cell>());
	
	g_fieldManager->m_rigidbodies.emplace_back(c);
	
	return c;
}
