#include "EggManager.h"
#include "Egg.h"
#include "Cell.h"
#include "FieldManager.h"

unique_ptr<EggManager> g_eggManagerPtr;

Indexer<EggState>& EggManager::GetIndexer()
{
	return m_indexer;
}

const Indexer<EggState>& EggManager::GetIndexer() const
{
	return m_indexer;
}

const shared_ptr<EggState>& EggManager::AddEggState(const shared_ptr<CellModel>& cellModel)
{
	const auto& m = GetEggStates().emplace_back(make_shared<EggState>());

	m->m_model = model;
	m->m_radius = model->m_radius;
	m->m_mass = model->m_mass;

	return m;
}
