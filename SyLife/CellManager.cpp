#include "CellManager.h"
#include "FieldManager.h"

unique_ptr<CellManager>	g_cellManagerPtr;

const shared_ptr<Cell>& CellManager::AddCell()
{
	const auto& c = GetCells().emplace_back(make_shared<Cell>());

	m_indexer.AddParticle(c);
	g_fieldManagerPtr->m_indexer.AddParticle(c);

	return c;
}