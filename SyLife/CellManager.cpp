#include "CellManager.h"
#include "Cell.h"
#include "FieldManager.h"

unique_ptr<CellManager>	g_cellManagerPtr;

const shared_ptr<CellState>& CellManager::AddCellState(const shared_ptr<CellModel>& model)
{
	const auto& c = GetCellStates().emplace_back(make_shared<CellState>(model));

	m_indexer.AddParticle(c);
	g_fieldManagerPtr->GetIndexer().AddParticle(c);

	return c;
}