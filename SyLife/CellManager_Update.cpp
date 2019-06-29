#include "CellManager.h"
#include "Cell.h"
#include "FieldManager.h"
#include "AssetManager.h"
#include "Molecule.h"
#include "MoleculeManager.h"

#include <boost/math/constants/constants.hpp>
#include "Random.h"
void CellManager::Update()
{
	for (const auto& c : GetCellStates())
	{
		if (c->m_isDestroyed) continue;

		// parts
		for (const auto& p : c->m_partStates) p->Update(*c);

		// 接触したMoleculeStateの取り込み
		for (const auto& m : g_moleculeManagerPtr->GetIndexer().GetNearParticles(c->m_position, c->m_radius * 2.0))
		{
			auto length = (m->m_position - c->m_position).length();

			if (!m->m_isDestroyed && length - m->m_radius - c->m_radius < 0.0) c->TakeMolecule(m);
		}

		// 余剰のMoleculeStateの投棄
		for (auto it = c->m_storage.m_molecules.begin(); it != c->m_storage.m_molecules.end(); ++it)
		{
			if ((*it).second > 10)
			{
				c->ExpireMolecule((*it).first);
				break;
			}
		}

		// 分裂処理
		if (c->m_storage >= c->m_model->m_material)
		{
			c->m_storage -= c->m_model->m_material;

			const auto& nc = g_cellManagerPtr->AddCellState(c->m_model);
			nc->m_position = c->m_position + Vector2D(1.0, 0.0).rotated(rand() / 360.0);
			nc->Init();
			nc->m_angularVelocity = (rand() % 1000 - 500) / 10000.0;
			nc->m_rotation = rand() / 360.0;
		}

		// 死亡処理
		c->m_deathTimer -= g_fieldManagerPtr->GetDeltaTime();
		if (c->m_deathTimer <= 0.0)
		{
			// MoleculeStateの吐き出し
			for (const auto& m : (c->m_storage + c->m_model->m_material).m_molecules)
			{
				for (unsigned int i = 0; i < m.second; i++)
				{
					auto pi = boost::math::constants::pi<double>();
					auto r = Random(pi * 2.0);
					auto v = Vector2D(1.0, 0.0).rotated(r);
					auto& t = g_moleculeManagerPtr->AddMoleculeState(m.first);

					t->m_position = c->m_position + v * (rand() % 100) / 100.0 * (c->m_radius + t->m_radius);
					t->m_velocity = v * 2.0;
				}
			}

			c->m_isDestroyed = true;
		}
	}

	GetCellStates().erase(remove_if(GetCellStates().begin(), GetCellStates().end(), [](const auto& c) { return c->m_isDestroyed; }), GetCellStates().end());
	m_indexer.Update();
}
