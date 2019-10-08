#include "EggManager.h"
#include "EggState.h"

unique_ptr<EggManager> g_eggManagerPtr;

const shared_ptr<EggState>& EggManager::addEggState(const shared_ptr<CellAsset>& cellModel)
{
	const auto& e = getEggStates().emplace_back(make_shared<EggState>());
	e->setCellAsset(cellModel);
	e->setMass(1.0);

	return e;
}

void EggManager::updateEggStates()
{
	for (const auto& e : getEggStates())
	{
		if (!e->isDestroyed())
		{
			e->updateParticle();
			e->updateEgg();
		}
	}

	m_eggStates.remove_if([](const auto& e) { return e->isDestroyed(); });
	m_eggStateKDTree.rebuildIndex();
}

void EggManager::drawEggStates()
{
	for (const auto& e : getEggStates())
		if (!e->isDestroyed()) e->draw();
}

EggStateAdapter::element_type EggStateAdapter::GetElement(const dataset_type& dataset, size_t index, size_t dim) { return dataset[index]->getPosition().elem(dim); }
EggStateAdapter::element_type EggStateAdapter::DistanceSq(const dataset_type& dataset, size_t index, const element_type* other) { return dataset[index]->getPosition().distanceFromSq(Vec2(other[0], other[1])); }
