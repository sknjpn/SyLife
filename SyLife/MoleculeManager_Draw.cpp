#include "MoleculeManager.h"

void	MoleculeManager::Draw()
{
	for (const auto& m : g_moleculeManagerPtr->GetMolecules())
	{
		s3d::Color color(m->m_model->m_color[0], m->m_model->m_color[1], m->m_model->m_color[2]);
		s3d::Circle(m->m_position.m_x, m->m_position.m_y, m->m_radius).draw(color).drawFrame(1.0, s3d::Palette::Gray);
	}
}