#include "Hotspot.h"

void HotspotState::UpdateHotspot()
{
	m_timer -= g_systemManagerPtr->GetDeltaTime();

	if (m_timer < 0.0)
	{

	}
}

void HotspotState::Draw()
{
	auto t = Transformer2D(Mat3x2::Translate(GetPosition()));

	for (int s = 100; s > 0; s -= 10)
		Circle(s).draw(ColorF(Palette::Darkred, 1.0 - s / 100.0));
}
