#pragma once

class EggState;
class CellAsset;

struct EggStateAdapter
{
	using dataset_type = Array<shared_ptr<EggState>>;
	using point_type = Vec2;
	using element_type = double;
	static constexpr int32 Dimensions = 2;

	static const element_type* GetPointer(const point_type& point) { return &point.x; }
	static element_type GetElement(const dataset_type& dataset, size_t index, size_t dim);
	static element_type DistanceSq(const dataset_type& dataset, size_t index, const element_type* other);
};

// EggStateの保存および更新を行う
class EggManager
{
	Array<shared_ptr<EggState>>	m_eggStates;
	KDTree<EggStateAdapter>		m_eggStateKDTree;

public:
	EggManager()
		: m_eggStateKDTree(m_eggStates)
	{
		m_eggStates.reserve(0xFFFF);
	}

	// Get
	Array<shared_ptr<EggState>>& getEggStates() { return m_eggStates; }
	const Array<shared_ptr<EggState>>& getEggStates() const { return m_eggStates; }
	const KDTree<EggStateAdapter>& getEggStateKDTree() const { return m_eggStateKDTree; }

	const shared_ptr<EggState>& addEggState(const shared_ptr<CellAsset>& cellModel);

	void	updateEggStates();
	void	drawEggStates();
};

extern unique_ptr<EggManager> g_eggManagerPtr;
