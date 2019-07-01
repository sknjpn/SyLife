#pragma once

#include "Viewer.h"

class WaveManager
{
	PerlinNoise perlinNoiseX;
	PerlinNoise perlinNoiseY;
	double	m_timer;

public:
	WaveManager()
		: m_timer(0.0)
		, perlinNoiseX(Random<int>(0, 99999))
		, perlinNoiseY(Random<int>(0, 99999))
	{}

	Vec2	GetWaveVelocity(const Vec2& position) const
	{
		auto interval = 200.0;
		auto x = perlinNoiseX.noise(Vec3(position.x / interval, position.y / interval, m_timer * 0.15));
		auto y = perlinNoiseY.noise(Vec3(position.x / interval, position.y / interval, m_timer * 0.15));

		return (Vec2(x, y) - position * 0.0001) * 100.0;
	}

	void	Update();
	void	Draw() const;
};

extern unique_ptr<WaveManager>	g_waveManagerPtr;
