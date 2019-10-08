#include "ElementManager.h"

unique_ptr<ElementManager> g_elementManagerPtr;

const shared_ptr<ElementState>& ElementManager::AddElementState(const shared_ptr<ElementModel>& model)
{
	const auto& m = m_newElementStates.emplace_back(make_shared<ElementState>());

	m->SetModel(model);

	return m;
}

const shared_ptr<ElementState>& ElementManager::AddElementState(const shared_ptr<ElementModel>& model, const Vec2& position)
{
	const auto& m = AddElementState(model);
	m->SetPosition(position);
	m->SetVelocity(RandomVec2(Random(100.0)));

	return m;
}

void ElementManager::UpdateElementStates()
{
	for (const auto& e : GetElementStates())
	{
		if (!e->IsDestroyed())
		{
			e->UpdateParticle();
			e->UpdateElement();
		}
	}

	// 存在しないElementの削除
	m_elementStates.remove_if([](const auto& m) { return m->IsDestroyed(); });

	// 新しいElementの追加
	m_elementStates.append(m_newElementStates);
	m_newElementStates.clear();

	m_elementStateKDTree.rebuildIndex();
}

void ElementManager::DrawElementStates()
{
	for (const auto& e : GetElementStates())
		if (!e->IsDestroyed()) e->Draw();
}

int ElementManager::NumElement(const shared_ptr<ElementModel>& model)
{
	return static_cast<int>(count_if(GetElementStates().begin(), GetElementStates().end(), [&model](const auto& m) { return m->GetModel() == model; }));
}

void ElementManager::AddElementsRandom(const shared_ptr<ElementModel>& model, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		auto p = RandomVec2(Random(1000));

		const auto& m = g_elementManagerPtr->AddElementState(model);
		m->SetPosition(Vec2(p.x, p.y));

		m->Init();
	}
}

ElementStateAdapter::element_type ElementStateAdapter::GetElement(const dataset_type& dataset, size_t index, size_t dim) { return dataset[index]->GetPosition().elem(dim); }
ElementStateAdapter::element_type ElementStateAdapter::DistanceSq(const dataset_type& dataset, size_t index, const element_type* other) { return dataset[index]->GetPosition().distanceFromSq(Vec2(other[0], other[1])); }
