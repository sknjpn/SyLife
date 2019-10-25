#include "WaveManager.h"

#include "SystemManager.h"
#include "ChipManager.h"

unique_ptr<WaveManager>	g_waveManagerPtr;

WaveManager::WaveManager()
	: m_timer(0.0)
	, perlinNoiseX(Random(0xFFFFFFFF))
	, perlinNoiseY(Random(0xFFFFFFFF))
{}

Vec2 WaveManager::getWaveVelocity(const Vec2& position) const
{
	auto interval = 400.0;
	//auto x = perlinNoiseX.noise(Vec3(position.x / interval, position.y / interval, m_timer * 0.05));
	//auto y = perlinNoiseY.noise(Vec3(position.x / interval, position.y / interval, m_timer * 0.05));
	auto x = perlinNoiseX.noise(Vec3(position.x / interval, position.y / interval, 0.5));
	auto y = perlinNoiseY.noise(Vec3(position.x / interval, position.y / interval, 0.5));

	return Vec2(x, y) * 100.0;
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

	const double interval = g_chipManagerPtr->getLength();
	for (auto p : step(g_chipManagerPtr->getSize()))
	{
		const double x = p.x * interval + interval / 2.0;
		const double y = p.y * interval + interval / 2.0;
		Line(Vec2(x, y), Vec2(x, y) + getWaveVelocity(Vec2(x, y)) * 2.0).drawArrow(20.0, Vec2(20.0, 20.0), ColorF(1.0, 0.2));
	}
}
