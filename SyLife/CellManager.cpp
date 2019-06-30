#include "CellManager.h"
#include "Cell.h"

unique_ptr<CellManager>	g_cellManagerPtr;

const shared_ptr<CellState>& CellManager::AddCellState(const shared_ptr<CellModel>& model)
{
	const auto& c = GetCellStates().emplace_back(make_shared<CellState>(model));

	m_indexer.AddParticle(c);
	g_fieldManagerPtr->GetIndexer().AddParticle(c);

	return c;
}

void	CellManager::Update()
{
	for (const auto& c : GetCellStates())
		if (!c->IsDestroyed()) c->UpdateCell();

	m_indexer.Update();
}

void	CellManager::Draw()
{
	for (const auto& c : GetCellStates()) 
		if (!c->IsDestroyed()) c->Draw();
}
