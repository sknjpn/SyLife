#pragma once

#include "Viewer.h"

class WaveManager
{
	s3d::PerlinNoise perlinNoiseX;
	s3d::PerlinNoise perlinNoiseY;
	double	m_timer;

public:
	WaveManager()
		: m_timer(0.0)
		, perlinNoiseX(s3d::Random<int>(0, 99999))
		, perlinNoiseY(s3d::Random<int>(0, 99999))
	{}

	s3d::Vec2	GetWaveVelocity(const s3d::Vec2& position) const
	{
		auto interval = 200.0;
		auto x = perlinNoiseX.noise(s3d::Vec3(position.x / interval, position.y / interval, m_timer * 0.15));
		auto y = perlinNoiseY.noise(s3d::Vec3(position.x / interval, position.y / interval, m_timer * 0.15));

		return (s3d::Vec2(x, y) - position * 0.0001) * 100.0;
	}

	void	Update();
	void	Draw() const;
};

extern unique_ptr<WaveManager>	g_waveManagerPtr;
