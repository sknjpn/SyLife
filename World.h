#pragma once

#include "TileState.h"

// State
class EggState;
class CellState;
class PartState;

// Asset
class Asset;
class CellAsset;
class PartAsset;
class ProteinAsset;
class PartConfig;

template <typename T>
struct ParticleAdapter
{
	using dataset_type = Array<std::shared_ptr<T>>;
	using point_type = Vec2;
	using element_type = double;
	static constexpr int32 Dimensions = 2;

	static const element_type* GetPointer(const point_type& point) { return &point.x; }
	static element_type GetElement(const dataset_type& dataset, size_t index, size_t dim) { return dataset[index]->getPosition().elem(dim); }
	static element_type DistanceSq(const dataset_type& dataset, size_t index, const element_type* other) { return dataset[index]->getPosition().distanceFromSq(Vec2(other[0], other[1])); }
};

class World
{
	Vec2	m_fieldSize;

	double	m_tileLength;

	DynamicTexture	m_tileTexture;

	double	m_waveInterval;
	double	m_elementPerTile;
	double	m_waveVelocityMax;

	// State
	Grid<TileState>		m_tiles;
	Grid<TileState>		m_tiles_swap;
	Array<Array<Point>>	m_tileGroups;

	Array<std::shared_ptr<CellState>>	m_cellStates;
	Array<std::shared_ptr<EggState>>		m_eggStates;
	KDTree<ParticleAdapter<CellState>>		m_cellStateKDTree;
	KDTree<ParticleAdapter<EggState>>		m_eggStateKDTree;

	String		m_name;
	FilePath	m_filePath;

	Array<std::shared_ptr<Asset>>	m_assets;

	static std::unique_ptr<World>	g_instance;

	void	loadAssets(const FilePath& directory);

	void	load();
	void	make();

	// Assets
	template <typename T>
	std::shared_ptr<T>		makeAsset() { return dynamic_pointer_cast<T>(m_assets.emplace_back(MakeShared<T>())); }
	std::shared_ptr<Asset>	makeAsset(const String& type);
	template <typename T>
	std::shared_ptr<T>		getAsset(const String& name) const
	{
		auto asset = dynamic_pointer_cast<T>(getAsset(name));

		if (asset) return asset;
		throw Error(U"存在しない名前のモデルを参照しました name:" + name);
	}
	std::shared_ptr<Asset>	getAsset(const String& name) const;
	template <typename T>
	Array<std::shared_ptr<T>>	getAssets() const
	{
		Array<std::shared_ptr<T>> tAssets;

		for (auto it = m_assets.begin(); it != m_assets.end(); ++it)
			if (dynamic_pointer_cast<T>(*it) != nullptr) tAssets.emplace_back(dynamic_pointer_cast<T>(*it));

		return tAssets;
	}
	bool	hasAsset(const String& name) const;

	void	updateTiles();
	void	updateTileGroup(int groupIndex);
	void	initTiles();

public:
	World();

	// Field
	const Array<std::shared_ptr<CellState>>& getCellStates() const { return m_cellStates; }
	const Array<std::shared_ptr<EggState>>& getEggStates() const { return m_eggStates; }
	const KDTree<ParticleAdapter<CellState>>& getCellStateKDTree() const { return m_cellStateKDTree; }
	const KDTree<ParticleAdapter<EggState>>& getEggStateKDTree() const { return m_eggStateKDTree; }
	const std::shared_ptr<CellState>& addCellState(const std::shared_ptr<CellAsset>& asset);
	const std::shared_ptr<EggState>& addEggState(const std::shared_ptr<CellAsset>& asset);
	TileState& getTile(const Point& point) { return m_tiles[point]; }
	const TileState& getTile(const Point& point) const { return m_tiles[point]; }
	TileState& getTile(const Vec2& position) { return m_tiles[(position / m_tileLength).asPoint()]; }
	const TileState& getTile(const Vec2& position) const { return m_tiles[(position / m_tileLength).asPoint()]; }
	const Grid<TileState>& getTiles() const { return m_tiles; }
	double	getTileLength() const { return m_tileLength; }
	const Vec2& getFieldSize() const { return m_fieldSize; }
	void	generateWave();

	// Assets
	template <typename T>
	static std::shared_ptr<T>		MakeAsset() { return dynamic_pointer_cast<T>(g_instance->m_assets.emplace_back(MakeShared<T>())); }
	static std::shared_ptr<Asset>	MakeAsset(const String& type) { return g_instance->makeAsset(type); }
	template <typename T>
	static std::shared_ptr<T>		GetAsset(const String& name) { return g_instance->getAsset<T>(name); }
	static std::shared_ptr<Asset>	GetAsset(const String& name) { return g_instance->getAsset(name); }
	static const Array<std::shared_ptr<Asset>>& GetAssets() { return g_instance->m_assets; }
	template <typename T>
	static Array<std::shared_ptr<T>>	GetAssets() { return g_instance->getAssets<T>(); }
	static bool	HasAsset(const String& name) { return g_instance->hasAsset(name); }
	static void	Remove(const std::shared_ptr<Asset>& asset) { g_instance->m_assets.remove(asset); }
	static void	Clear() { g_instance->m_assets.clear(); }

	static void Make();
	static void Load(const FilePath& filepath);
	static const std::unique_ptr<World>& GetInstance() { return g_instance; }

	void	setName(const String& name) { m_name = name; }
	void	setFilePath(const FilePath& filePath) { m_filePath = filePath; }

	static FilePath	GetDirectory() { return g_instance->m_filePath; }

	void	update();
	void	draw();

	void	save();

	void	initField();
};
