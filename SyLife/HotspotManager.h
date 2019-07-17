#pragma once

class HotspotModel;
class HotspotConfig;
class HotspotState;

struct HotspotStateAdapter
{
	using dataset_type = Array<shared_ptr<HotspotState>>;
	using point_type = Vec2;
	using element_type = double;
	static constexpr int32 Dimensions = 2;

	static const element_type* GetPointer(const point_type& point) { return &point.x; }
	static element_type GetElement(const dataset_type& dataset, size_t index, size_t dim);
	static element_type DistanceSq(const dataset_type& dataset, size_t index, const element_type* other);
};

class HotspotManager
{
	Array<shared_ptr<HotspotState>>	m_hotspotStates;
	KDTree<HotspotStateAdapter>		m_hotspotStateKDTree;

public:
	HotspotManager()
		: m_hotspotStateKDTree(m_hotspotStates)
	{
		m_hotspotStates.reserve(0xFFFF);
	}

	// const shared_ptr<HotspotState>& AddHotspotState(const shared_ptr<HotspotModel>& model);
	// const shared_ptr<HotspotState>& AddHotspotState(const shared_ptr<HotspotModel>& model, const Vec2& position);

	// Get
	Array<shared_ptr<HotspotState>>& GetHotspotStates() { return m_hotspotStates; }
	const Array<shared_ptr<HotspotState>>& GetHotspotStates() const { return m_hotspotStates; }
	const KDTree<HotspotStateAdapter>& GetHotspotStateKDTree() const { return m_hotspotStateKDTree; }

	shared_ptr<HotspotState>&	MakeHotspotState(const shared_ptr<HotspotModel>& hotspotModel);
	shared_ptr<HotspotState>&	MakeHotspotState(const shared_ptr<HotspotConfig>& hotspotConfig);
	void	MakeHotspots();

	void	Init();
	void	UpdateHotspotStates();
	void	DrawHotspotStates();
};

extern unique_ptr<HotspotManager> g_hotspotManagerPtr;
