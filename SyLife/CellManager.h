#pragma once

class CellState;
class CellModel;

struct CellStateAdapter
{
	using dataset_type = Array<shared_ptr<CellState>>;
	using point_type = Vec2;
	using element_type = double;
	static constexpr int32 Dimensions = 2;

	static const element_type* GetPointer(const point_type& point) { return &point.x; }
	static element_type GetElement(const dataset_type& dataset, size_t index, size_t dim);
	static element_type DistanceSq(const dataset_type& dataset, size_t index, const element_type* other);
};

// CellStateの保存および更新を行う
class CellManager
{
	Array<shared_ptr<CellState>>	m_cellStates;
	KDTree<CellStateAdapter>		m_cellStateKDTree;

public:
	CellManager()
		: m_cellStateKDTree(m_cellStates)
	{
		m_cellStates.reserve(0xFFFF);
	}

	// Get
	Array<shared_ptr<CellState>>& GetCellStates() { return m_cellStates; }
	const Array<shared_ptr<CellState>>& GetCellStates() const { return m_cellStates; }
	KDTree<CellStateAdapter>& GetCellStateKDTree() { return m_cellStateKDTree; }
	const KDTree<CellStateAdapter>& GetCellStateKDTree() const { return m_cellStateKDTree; }

	const shared_ptr<CellState>& AddCellState(const shared_ptr<CellModel>& model);

	void	UpdateCellStates();
	void	DrawCellStates();
};

extern unique_ptr<CellManager>	g_cellManagerPtr;
