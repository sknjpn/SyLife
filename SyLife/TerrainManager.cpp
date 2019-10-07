#include "TerrainManager.h"

#include "Terrain.h"
#include "Hotspot.h"

unique_ptr<TerrainManager>	g_terrainManagerPtr;

double TerrainManager::GetHeight(const Vec2& position) const
{
	auto n = abs(m_noise.octaveNoise(position / m_terrainModel->m_size * m_terrainModel->m_density, 7));
	auto m = EaseInOut(Easing::Sine, 0.0, 1.0, n);
	auto l = Min((position / m_terrainModel->m_size).length() * 2.0, 1.0);

	return EaseIn(Easing::Expo, m, 0.0, l);
}

Color TerrainManager::getColor(const Vec2& position)
{
	return Color(int(GetHeight(position) * 30), 0, 0);

	//return Math::Lerp(Color(11, 22, 33), Palette::Cyan, int(GetHeight(position) * 10.0) / 10.0);
}

void TerrainManager::MakeTexture(int textureSize)
{
	auto image = Image(textureSize, textureSize);

	for (int x = 0; x < textureSize - 1; x += 2)
	{
		for (int y = 0; y < textureSize - 1; y += 2)
		{
			auto color = getColor(Vec2(x - textureSize / 2, y - textureSize / 2) * m_terrainModel->m_size / double(textureSize));

			image[y][x] = color;
			image[y][x + 1] = color;
			image[y + 1][x] = color;
			image[y + 1][x + 1] = color;
		}
	}

	for (int i = 20; i >= 0; --i)
		m_polygons.emplace_back(image.grayscaleToPolygons(i));

	m_polygons.reverse();

	for (auto& p : m_polygons)
	{
		p = p.simplified(2.0);
		p.scale(m_terrainModel->m_size / double(textureSize));
		p.moveBy(-Vec2::One() * m_terrainModel->m_size / 2.0);
	}
	//m_texture = Texture(image);
}

void TerrainManager::SetTerrainModel(const shared_ptr<TerrainModel>& model)
{
	m_terrainModel = model;
	m_noise = PerlinNoise(m_terrainModel->m_noiseSeed);

	// MakeTexture(0x1000);
	// MakeHotspots(100);
}

void TerrainManager::UpdateTerrain()
{

}

void TerrainManager::DrawTerrain()
{
	if (m_polygons.empty()) return;

	for (int i = 0; i <= 20; ++i)
		m_polygons[i].draw(Math::Lerp(Color(11, 22, 33), Palette::Cyan, i / 20.0));
}
