#pragma once

#include "Viewer.h"

// ある点での波の大きさの共有と更新
class WaveManager
{
	PerlinNoise perlinNoiseX;
	PerlinNoise perlinNoiseY;
	double	m_timer;

public:
	WaveManager();

	Vec2	GetWaveVelocity(const Vec2& position) const;

	void	Update();
	void	Draw() const;
};

extern unique_ptr<WaveManager>	g_waveManagerPtr;
