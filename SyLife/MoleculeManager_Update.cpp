#include "MoleculeManager.h"
#include "FieldManager.h"

void MoleculeManager::Update()
{
	for (const auto& m : GetMolecules())
	{
		if (m->m_model == GetModel("Amino Acid") && rand() % 100 == 0)
		{
			AddMolecule(GetModel("Nitrogen"), m->m_position + Vector2D(1.0, 0.0).rotated(rand() / 360.0));
			AddMolecule(GetModel("Carbon"), m->m_position + Vector2D(1.0, 0.0).rotated(rand() / 360.0));
			AddMolecule(GetModel("Oxygen"), m->m_position + Vector2D(1.0, 0.0).rotated(rand() / 360.0));

			m->m_destroyFlag = true;
		}

		// •Ài‰^“®
		m->m_position += m->m_velocity;

		// •Ç‚Å‚Ì”½ŽË
		if (m->m_position.m_x < 0 && m->m_velocity.m_x < 0) m->m_velocity.m_x = -m->m_velocity.m_x;
		if (m->m_position.m_y < 0 && m->m_velocity.m_y < 0) m->m_velocity.m_y = -m->m_velocity.m_y;
		if (m->m_position.m_x > g_fieldManagerPtr->m_size.m_x && m->m_velocity.m_x > 0) m->m_velocity.m_x = -m->m_velocity.m_x;
		if (m->m_position.m_y > g_fieldManagerPtr->m_size.m_y && m->m_velocity.m_y > 0) m->m_velocity.m_y = -m->m_velocity.m_y;

		// –€ŽC’ïR
		m->m_velocity /= (1.0 + m->m_radius * 0.001);
	}

	GetMolecules().erase(remove_if(GetMolecules().begin(), GetMolecules().end(), [](const auto& m) { return m->m_destroyFlag; }), GetMolecules().end());
	m_indexer.Update();
}
