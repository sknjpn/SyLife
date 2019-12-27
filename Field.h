#pragma once

#include "CellState.h"
#include "ElementState.h"
#include "EggState.h"
#include "ChipState.h"

class CellAsset;
class ElementAsset;

template <typename T>
struct ParticleAdapter
{
	using dataset_type = Array<shared_ptr<T>>;
	using point_type = Vec2;
	using element_type = double;
	static constexpr int32 Dimensions = 2;

	static const element_type* GetPointer(const point_type& point) { return &point.x; }
	static element_type GetElement(const dataset_type& dataset, size_t index, size_t dim) { return dataset[index]->getPosition().elem(dim); }
	static element_type DistanceSq(const dataset_type& dataset, size_t index, const element_type* other) { return dataset[index]->getPosition().distanceFromSq(Vec2(other[0], other[1])); }
};

class Field
{
	// State
	Array<shared_ptr<CellState>>	m_cellStates;
	Array<shared_ptr<EggState>>		m_eggStates;
	Array<shared_ptr<ElementState>>	m_elementStates;
	KDTree<ParticleAdapter<CellState>>		m_cellStateKDTree;
	KDTree<ParticleAdapter<EggState>>		m_eggStateKDTree;
	KDTree<ParticleAdapter<ElementState>>	m_elementStateKDTree;

	// Chip
	Grid<shared_ptr<TileState>> m_chips;
	Size	m_chipSize;
	double	m_chipLength;

	Vec2	m_fieldSize;

public:
	Field();

	void	update();
	void	draw();

	// Get
	const Array<shared_ptr<CellState>>& getCellStates() const { return m_cellStates; }
	const Array<shared_ptr<EggState>>& getEggStates() const { return m_eggStates; }
	const Array<shared_ptr<ElementState>>& getElementStates() const { return m_elementStates; }
	const KDTree<ParticleAdapter<CellState>>& getCellStateKDTree() const { return m_cellStateKDTree; }
	const KDTree<ParticleAdapter<EggState>>& getEggStateKDTree() const { return m_eggStateKDTree; }
	const KDTree<ParticleAdapter<ElementState>>& getElementStateKDTree() const { return m_elementStateKDTree; }

	const shared_ptr<CellState>& addCellState(const shared_ptr<CellAsset>& asset) { return m_cellStates.emplace_back(make_shared<CellState>(asset)); }
	const shared_ptr<EggState>& addEggState(const shared_ptr<CellAsset>& asset) { return m_eggStates.emplace_back(make_shared<EggState>(asset));}
	const shared_ptr<ElementState>& addElementState(const shared_ptr<ElementAsset>& asset) { return m_elementStates.emplace_back(make_shared<ElementState>(asset)); }

	Point	getPoint(const Vec2& position) const { return Point(int(position.x / m_chipLength), int(position.y / m_chipLength)); }
	shared_ptr<TileState> getChip(const Point& point) const;
	shared_ptr<TileState> getChip(const Vec2& position) const { return getChip(getPoint(position)); }
	const Grid<shared_ptr<TileState>>& getChips() const { return m_chips; }
	double	getChipLength() const { return m_chipLength; }
	const Size& getChipSize() const { return m_chipSize; }
	const Vec2& getFieldSize() const { return m_fieldSize; }

	void	save(const FilePath& directory);
	void	load(const FilePath& directory);

	void	generateWave(const Size& chipSize);

	void	init();

};
