#pragma once

class TerrainModel;

class TerrainManager
{
	shared_ptr<TerrainModel> m_terrainModel;

public:

	void	Update();
	void	Draw();
};


