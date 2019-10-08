#include "WaveManager.h"

#include "SystemManager.h"
#include "TerrainManager.h"

#include "Terrain.h"

unique_ptr<WaveManager>	g_waveManagerPtr;

WaveManager::WaveManager()
	: m_timer(0.0)
	, perlinNoiseX(Random(0xFFFFFFFF))
	, perlinNoiseY(Random(0xFFFFFFFF))
{}

Vec2 WaveManager::getWaveVelocity(const Vec2& position) const
{
	//return (-position * 0.0001) * 100.0;

	auto interval = 200.0;
	auto x = perlinNoiseX.noise(Vec3(position.x / interval, position.y / interval, m_timer * 0.15));
	auto y = perlinNoiseY.noise(Vec3(position.x / interval, position.y / interval, m_timer * 0.15));

	return Vec2(x, y);
}

void WaveManager::updateWave()
{
	m_timer += g_systemManagerPtr->GetDeltaTime();
}

void WaveManager::drawWave() const
{
	static bool showWave = false;
	if (KeyP.down()) showWave = !showWave;

	if (!showWave) return;

	double interval = 200;
	double size = g_terrainManagerPtr->GetTerrainModel()->m_size / 2.0;

	for (double x = -size; x <= size; x += interval)
		for (double y = -size; y <= size; y += interval)
			Line(Vec2(x, y), Vec2(x, y) + getWaveVelocity(Vec2(x, y)) * 2.0).drawArrow(20.0, Vec2(20.0, 20.0), ColorF(1.0, 0.2));
}
