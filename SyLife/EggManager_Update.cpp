#include "EggManager.h"
#include "FieldManager.h"
#include "AssetManager.h"
#include "CellManager.h"
#include "Cell.h"
#include "Egg.h"

void EggManager::Update()
{
	for (const auto& e : GetEggStates())
	{
		// Update
		e->Update();

		// •Ài‰^“®
		e->m_position += e->m_velocity;

		// ‰ñ“]‰^“®
		e->m_rotation += e->m_angularVelocity;

		// •Ç‚Å‚Ì”½ŽË
		if (e->m_position.m_x < 0 && e->m_velocity.m_x < 0) e->m_velocity.m_x = -e->m_velocity.m_x;
		if (e->m_position.m_y < 0 && e->m_velocity.m_y < 0) e->m_velocity.m_y = -e->m_velocity.m_y;
		if (e->m_position.m_x > g_fieldManagerPtr->GetSize().m_x && e->m_velocity.m_x > 0) e->m_velocity.m_x = -e->m_velocity.m_x;
		if (e->m_position.m_y > g_fieldManagerPtr->GetSize().m_y && e->m_velocity.m_y > 0) e->m_velocity.m_y = -e->m_velocity.m_y;

		// –€ŽC’ïR
		e->m_velocity /= (1.0 + e->m_radius * 0.001);
		e->m_angularVelocity /= (1.0 + e->m_radius * 0.001);
	}

	GetEggStates().erase(remove_if(GetEggStates().begin(), GetEggStates().end(), [](const auto& e) { return e->m_isDestroyed; }), GetEggStates().end());
	m_indexer.Update();
}
