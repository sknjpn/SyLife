#include "EggManager.h"
#include "FieldManager.h"
#include "AssetManager.h"
#include "Egg.h"

void EggManager::Update()
{
	for (const auto& m : GetEggStates())
	{
		// •Ài‰^“®
		m->m_position += m->m_velocity;

		// •Ç‚Å‚Ì”½ŽË
		if (m->m_position.m_x < 0 && m->m_velocity.m_x < 0) m->m_velocity.m_x = -m->m_velocity.m_x;
		if (m->m_position.m_y < 0 && m->m_velocity.m_y < 0) m->m_velocity.m_y = -m->m_velocity.m_y;
		if (m->m_position.m_x > g_fieldManagerPtr->GetSize().m_x && m->m_velocity.m_x > 0) m->m_velocity.m_x = -m->m_velocity.m_x;
		if (m->m_position.m_y > g_fieldManagerPtr->GetSize().m_y && m->m_velocity.m_y > 0) m->m_velocity.m_y = -m->m_velocity.m_y;

		// –€ŽC’ïR
		m->m_velocity *= 0.95;
	}

	GetEggStates().erase(remove_if(GetEggStates().begin(), GetEggStates().end(), [](const auto& m) { return m->m_isDestroyed; }), GetEggStates().end());
	m_indexer.Update();
}
