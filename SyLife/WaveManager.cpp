#include "WaveManager.h"
#include "FieldManager.h"

unique_ptr<WaveManager>	g_waveManagerPtr;

void WaveManager::Update()
{
	m_timer += g_fieldManagerPtr->GetDeltaTime();
}

void WaveManager::Draw() const
{
	double interval = 50;
	double length = 50.0;

	for (double x = interval / 2.0; x < g_fieldManagerPtr->GetSize().m_x; x += interval)
	{
		for (double y = interval / 2.0; y < g_fieldManagerPtr->GetSize().m_y; y += interval)
		{
			auto v = GetWaveVelocity(Vector2D(x, y));

			s3d::Line(x, y, x + v.m_x * length, y + v.m_y * length).drawArrow(3.0, s3d::Vec2(6.0, 6.0), s3d::ColorF(1.0, 0.2));
		}
	}
}
