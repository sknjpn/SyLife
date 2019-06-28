#include "EggManager.h"
#include "FieldManager.h"
#include "AssetManager.h"
#include "CellManager.h"
#include "Cell.h"
#include "Egg.h"
#include "Random.h"

#include <boost/math/constants/constants.hpp>

void EggManager::Update()
{
	for (const auto& e : GetEggStates())
	{
		// •Ài‰^“®
		e->m_position += e->m_velocity;

		// •Ç‚Å‚Ì”½ŽË
		if (e->m_position.m_x < 0 && e->m_velocity.m_x < 0) e->m_velocity.m_x = -e->m_velocity.m_x;
		if (e->m_position.m_y < 0 && e->m_velocity.m_y < 0) e->m_velocity.m_y = -e->m_velocity.m_y;
		if (e->m_position.m_x > g_fieldManagerPtr->GetSize().m_x && e->m_velocity.m_x > 0) e->m_velocity.m_x = -e->m_velocity.m_x;
		if (e->m_position.m_y > g_fieldManagerPtr->GetSize().m_y && e->m_velocity.m_y > 0) e->m_velocity.m_y = -e->m_velocity.m_y;

		// –€ŽC’ïR
		e->m_velocity *= 0.95;

		// ›z‰»
		if (RandomBool(0.01))
		{
			e->m_isDestroyed = true;
			
			const auto& c = g_cellManagerPtr->AddCellState(e->m_cellModel);
			c->m_position = e->m_position;
			c->m_rotation = Random(boost::math::constants::pi<double>() * 2.0);
		}
	}

	GetEggStates().erase(remove_if(GetEggStates().begin(), GetEggStates().end(), [](const auto& e) { return e->m_isDestroyed; }), GetEggStates().end());
	m_indexer.Update();
}
