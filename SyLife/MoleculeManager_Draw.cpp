#include "MoleculeManager.h"
#include "Molecule.h"

void	MoleculeManager::Draw()
{
	for (const auto& m : g_moleculeManagerPtr->GetMoleculeStates())
	{
		s3d::Circle(m->m_position.m_x, m->m_position.m_y, m->m_radius)
			.draw(s3d::ColorF(m->m_model->m_color, 0.5))
			.drawFrame(1.0, s3d::ColorF(1.0, 0.5));
	}
}