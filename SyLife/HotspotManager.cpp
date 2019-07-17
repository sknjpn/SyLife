#include "Hotspot.h"

unique_ptr<HotspotManager> g_hotspotManagerPtr;

/*
const shared_ptr<HotspotState>& HotspotManager::AddHotspotState(const shared_ptr<HotspotModel>& model)
{
	const auto& m = GetHotspotStates().emplace_back(make_shared<HotspotState>());

	m->SetModel(model);

	return m;
}

const shared_ptr<HotspotState>& HotspotManager::AddHotspotState(const shared_ptr<HotspotModel>& model, const Vec2& position)
{
	const auto& m = AddHotspotState(model);
	m->SetPosition(position);
	m->SetVelocity(RandomVec2(Random(100.0)));

	return m;
}
*/

HotspotStateAdapter::element_type HotspotStateAdapter::GetElement(const dataset_type& dataset, size_t index, size_t dim) { return dataset[index]->GetPosition().elem(dim); }
HotspotStateAdapter::element_type HotspotStateAdapter::DistanceSq(const dataset_type& dataset, size_t index, const element_type* other) { return dataset[index]->GetPosition().distanceFromSq(Vec2(other[0], other[1])); }
