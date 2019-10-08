#include "CellManager.h"

#include "CellState.h"

unique_ptr<CellManager>	g_cellManagerPtr;

const shared_ptr<CellState>& CellManager::AddCellState(const shared_ptr<CellModel>& model)
{
	return m_cellStates.emplace_back(make_shared<CellState>(model));
}

void	CellManager::UpdateCellStates()
{
	for (const auto& c : m_cellStates)
	{
		if (!c->IsDestroyed())
		{
			c->UpdateParticle();
			c->UpdateRigidbody();
			c->UpdateCell();
		}
	}

	m_cellStates.remove_if([](const auto& c) { return c->IsDestroyed(); });
	m_cellStateKDTree.rebuildIndex();
}

void	CellManager::DrawCellStates()
{
	for (const auto& c : m_cellStates)
		if (!c->IsDestroyed()) c->draw();
}

CellStateAdapter::element_type CellStateAdapter::GetElement(const dataset_type& dataset, size_t index, size_t dim) { return dataset[index]->getPosition().elem(dim); }
CellStateAdapter::element_type CellStateAdapter::DistanceSq(const dataset_type& dataset, size_t index, const element_type* other) { return dataset[index]->getPosition().distanceFromSq(Vec2(other[0], other[1])); }
