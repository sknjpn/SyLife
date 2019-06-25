#include "MoleculeManager.h"
#include "Molecule.h"

void	MoleculeManager::Draw()
{
	for (const auto& m : g_moleculeManagerPtr->GetMoleculeStates())
	{
		s3d::Circle(m->m_position.m_x, m->m_position.m_y, m->m_radius)
			.draw(m->m_model->m_color)
			.drawFrame(1.0, s3d::Palette::Gray);
	}
}