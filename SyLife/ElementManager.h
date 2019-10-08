#pragma once

class ElementState;
class ElementModel;

struct ElementStateAdapter
{
	using dataset_type = Array<shared_ptr<ElementState>>;
	using point_type = Vec2;
	using element_type = double;
	static constexpr int32 Dimensions = 2;

	static const element_type* GetPointer(const point_type& point) { return &point.x; }
	static element_type GetElement(const dataset_type& dataset, size_t index, size_t dim);
	static element_type DistanceSq(const dataset_type& dataset, size_t index, const element_type* other);
};

// ElementStateの保存および更新を行う
class ElementManager
{
	Array<shared_ptr<ElementState>>	m_elementStates;
	Array<shared_ptr<ElementState>>	m_newElementStates;
	KDTree<ElementStateAdapter>		m_elementStateKDTree;

public:
	ElementManager()
		: m_elementStateKDTree(m_elementStates)
	{
		m_elementStates.reserve(0xFFFF);
	}

	int NumElement(const shared_ptr<ElementModel>& model);

	const shared_ptr<ElementState>& addElementState(const shared_ptr<ElementModel>& model);
	const shared_ptr<ElementState>& addElementState(const shared_ptr<ElementModel>& model, const Vec2& position);

	void	addElementsRandom(const shared_ptr<ElementModel>& model, size_t size);

	// Get
	const Array<shared_ptr<ElementState>>& getElementStates() const { return m_elementStates; }
	const KDTree<ElementStateAdapter>& getElementStateKDTree() const { return m_elementStateKDTree; }

	void	updateElementStates();
	void	drawElementStates();
};

extern unique_ptr<ElementManager> g_elementManagerPtr;
