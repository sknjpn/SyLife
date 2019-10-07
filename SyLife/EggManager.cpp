#include "EggManager.h"
#include "EggState.h"

unique_ptr<EggManager> g_eggManagerPtr;

const shared_ptr<EggState>& EggManager::AddEggState(const shared_ptr<CellModel>& cellModel)
{
	const auto& e = GetEggStates().emplace_back(make_shared<EggState>());
	e->SetCellModel(cellModel);
	e->SetMass(1.0);

	return e;
}

void EggManager::UpdateEggStates()
{
	for (const auto& e : GetEggStates())
	{
		if (!e->IsDestroyed())
		{
			e->UpdateParticle();
			e->UpdateEgg();
		}
	}

	m_eggStates.remove_if([](const auto& e) { return e->IsDestroyed(); });
	m_eggStateKDTree.rebuildIndex();
}

void EggManager::DrawEggStates()
{
	for (const auto& e : GetEggStates())
		if (!e->IsDestroyed()) e->Draw();
}

EggStateAdapter::element_type EggStateAdapter::GetElement(const dataset_type& dataset, size_t index, size_t dim) { return dataset[index]->getPosition().elem(dim); }
EggStateAdapter::element_type EggStateAdapter::DistanceSq(const dataset_type& dataset, size_t index, const element_type* other) { return dataset[index]->getPosition().distanceFromSq(Vec2(other[0], other[1])); }
