#include "ElementManager.h"

#include "ElementState.h"

unique_ptr<ElementManager> g_elementManagerPtr;

const shared_ptr<ElementState>& ElementManager::addElementState(const shared_ptr<ElementAsset>& asset)
{
	const auto& m = m_newElementStates.emplace_back(make_shared<ElementState>());

	m->setElementAsset(asset);

	return m;
}

const shared_ptr<ElementState>& ElementManager::addElementState(const shared_ptr<ElementAsset>& asset, const Vec2& position)
{
	const auto& m = addElementState(asset);
	m->setPosition(position);
	m->setVelocity(RandomVec2(Random(100.0)));

	return m;
}

void ElementManager::updateElementStates()
{
	for (const auto& e : getElementStates())
	{
		if (!e->isDestroyed())
		{
			e->updateParticle();
			e->updateElement();
		}
	}

	// 存在しないElementの削除
	m_elementStates.remove_if([](const auto& m) { return m->isDestroyed(); });

	// 新しいElementの追加
	m_elementStates.append(m_newElementStates);
	m_newElementStates.clear();

	m_elementStateKDTree.rebuildIndex();
}

void ElementManager::drawElementStates()
{
	for (const auto& e : getElementStates())
		if (!e->isDestroyed()) e->draw();
}

int ElementManager::NumElement(const shared_ptr<ElementAsset>& asset)
{
	return static_cast<int>(count_if(getElementStates().begin(), getElementStates().end(), [&asset](const auto& m) { return m->getModel() == asset; }));
}

void ElementManager::addElementsRandom(const shared_ptr<ElementAsset>& asset, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		auto p = RandomVec2(Random(1000));

		const auto& m = g_elementManagerPtr->addElementState(asset);
		m->setPosition(Vec2(p.x, p.y));

		m->init();
	}
}

ElementStateAdapter::element_type ElementStateAdapter::GetElement(const dataset_type& dataset, size_t index, size_t dim) { return dataset[index]->getPosition().elem(dim); }
ElementStateAdapter::element_type ElementStateAdapter::DistanceSq(const dataset_type& dataset, size_t index, const element_type* other) { return dataset[index]->getPosition().distanceFromSq(Vec2(other[0], other[1])); }
