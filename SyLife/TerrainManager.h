#pragma once

class TerrainModel;
class HotspotState;

class TerrainManager
{
	shared_ptr<TerrainModel> m_terrainModel;

	// ê∂ê¨Ç≥ÇÍÇÈÇ‡ÇÃ
	Texture	m_texture;
	PerlinNoise	m_noise;
	vector<shared_ptr<HotspotState>>	m_hotspots;
	Array<MultiPolygon>	m_polygons;

public:

	double	GetHeight(const Vec2& position) const;
	Color	GetColor(const Vec2& position);

	void	MakeTexture(int textureSize = 2048);
	void	MakeHotspots(int numHotspots = 300);

	void	SetTerrainModel(const shared_ptr<TerrainModel>& model);
	const shared_ptr<TerrainModel>&	GetTerrainModel() { return m_terrainModel; }

	void	Update();
	void	Draw();
};

extern unique_ptr<TerrainManager>	g_terrainManagerPtr;
