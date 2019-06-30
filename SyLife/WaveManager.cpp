#include "WaveManager.h"
#include "FieldManager.h"

unique_ptr<WaveManager>	g_waveManagerPtr;

void WaveManager::Update()
{
	m_timer += g_fieldManagerPtr->GetDeltaTime();
}

void WaveManager::Draw() const
{
	double interval = 200;
	double length = 200.0;
	double size = 4000;

	for (double x = -size; x < size; x += interval)
	{
		for (double y = -size; y < size; y += interval)
		{
			auto v = GetWaveVelocity(Vector2D(x, y)) * 0.01;

			s3d::Line(x, y, x + v.m_x * length, y + v.m_y * length).drawArrow(20.0, s3d::Vec2(20.0, 20.0), s3d::ColorF(1.0, 0.2));
		}
	}
}
