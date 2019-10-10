#include "WaveManager.h"

#include "SystemManager.h"

unique_ptr<WaveManager>	g_waveManagerPtr;

WaveManager::WaveManager()
	: m_timer(0.0)
	, perlinNoiseX(Random(0xFFFFFFFF))
	, perlinNoiseY(Random(0xFFFFFFFF))
{}

Vec2 WaveManager::getWaveVelocity(const Vec2& position) const
{
	//return (-position * 0.0001) * 100.0;

	auto interval = 300.0;
	//auto x = perlinNoiseX.noise(Vec3(position.x / interval, position.y / interval, m_timer * 0.05));
	//auto y = perlinNoiseY.noise(Vec3(position.x / interval, position.y / interval, m_timer * 0.05));
	auto x = perlinNoiseX.noise(Vec3(position.x / interval, position.y / interval, 0.5));
	auto y = perlinNoiseY.noise(Vec3(position.x / interval, position.y / interval, 0.5));

	return Vec2(x, y) * 50.0;
}

void WaveManager::updateWave()
{
	m_timer += g_systemManagerPtr->GetDeltaTime();
}

void WaveManager::drawWave() const
{
	/*
	static bool showWave = false;
	if (KeyP.down()) showWave = !showWave;

	if (!showWave) return;

	double interval = 200;
	double size = g_terrainManagerPtr->GetTerrainModel()->m_size / 2.0;

	for (double x = -size; x <= size; x += interval)
		for (double y = -size; y <= size; y += interval)
			Line(Vec2(x, y), Vec2(x, y) + getWaveVelocity(Vec2(x, y)) * 2.0).drawArrow(20.0, Vec2(20.0, 20.0), ColorF(1.0, 0.2));
	*/
}
