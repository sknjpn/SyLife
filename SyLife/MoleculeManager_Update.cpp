#include "MoleculeManager.h"
#include "FieldManager.h"
#include "AssetManager.h"
#include "Molecule.h"

void MoleculeManager::Update()
{
	for (const auto& m : GetMoleculeStates())
	{
		// Amino Acid‚Ì•ª‰ð
		if (m->m_model == g_assetManagerPtr->GetModel<MoleculeModel>("Amino Acid") && rand() % 100 == 0)
		{
			AddMoleculeState(g_assetManagerPtr->GetModel<MoleculeModel>("Nitrogen"), m->m_position + Vector2D(20.0, 0.0).rotated(rand() / 360.0));
			AddMoleculeState(g_assetManagerPtr->GetModel<MoleculeModel>("Carbon"), m->m_position + Vector2D(20.0, 0.0).rotated(rand() / 360.0));
			AddMoleculeState(g_assetManagerPtr->GetModel<MoleculeModel>("Oxygen"), m->m_position + Vector2D(20.0, 0.0).rotated(rand() / 360.0));

			m->m_isDestroyed = true;

			continue;
		}

		// •Ài‰^“®
		m->m_position += m->m_velocity;

		// •Ç‚Å‚Ì”½ŽË
		if (m->m_position.m_x < 0 && m->m_velocity.m_x < 0) m->m_velocity.m_x = -m->m_velocity.m_x;
		if (m->m_position.m_y < 0 && m->m_velocity.m_y < 0) m->m_velocity.m_y = -m->m_velocity.m_y;
		if (m->m_position.m_x > g_fieldManagerPtr->GetSize().m_x && m->m_velocity.m_x > 0) m->m_velocity.m_x = -m->m_velocity.m_x;
		if (m->m_position.m_y > g_fieldManagerPtr->GetSize().m_y && m->m_velocity.m_y > 0) m->m_velocity.m_y = -m->m_velocity.m_y;

		// –€ŽC’ïR
		m->m_velocity /= (1.0 + m->m_radius * 0.001);
	}

	GetMoleculeStates().erase(remove_if(GetMoleculeStates().begin(), GetMoleculeStates().end(), [](const auto& m) { return m->m_isDestroyed; }), GetMoleculeStates().end());
	m_indexer.Update();
}
