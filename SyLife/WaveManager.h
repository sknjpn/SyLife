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

	Vector2D	GetWaveVelocity(const Vector2D& position) const
	{
		auto interval = 200.0;
		auto x = perlinNoiseX.noise(s3d::Vec3(position.m_x / interval, position.m_y / interval, m_timer * 0.15));
		auto y = perlinNoiseY.noise(s3d::Vec3(position.m_x / interval, position.m_y / interval, m_timer * 0.15));

		return Vector2D(x, y) * 2.0;
	}

	void	Update();
	void	Draw() const;
};

extern unique_ptr<WaveManager>	g_waveManagerPtr;
