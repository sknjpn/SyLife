unique_ptr<WaveManager>	g_waveManagerPtr;

void WaveManager::Update()
{
	m_timer += g_fieldManagerPtr->GetDeltaTime();
}

void WaveManager::Draw() const
{
	static bool showWave = false;
	if (KeyP.down()) showWave = !showWave;

	if (!showWave) return;

	double interval = 200;
	double length = 200.0;
	double size = 4000;

	for (double x = -size; x < size; x += interval)
	{
		for (double y = -size; y < size; y += interval)
		{
			auto v = GetWaveVelocity(Vec2(x, y)) * 0.01;

			Line(x, y, x + v.x * length, y + v.y * length).drawArrow(20.0, Vec2(20.0, 20.0), ColorF(1.0, 0.2));
		}
	}
}
