#include "EggManager.h"
#include "Egg.h"
#include "Cell.h"

void EggManager::Draw()
{
	for (const auto& e : g_eggManagerPtr->GetEggStates())
	{
		s3d::Circle(e->m_position.m_x, e->m_position.m_y, e->m_cellModel->m_radius / 4.0)
			.draw(s3d::ColorF(s3d::Palette::Papayawhip, 0.5))
			.drawFrame(1.0, s3d::ColorF(1.0, 0.5));
	}
}