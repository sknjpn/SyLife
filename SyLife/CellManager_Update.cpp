#include "CellManager.h"
#include "Cell.h"
#include "FieldManager.h"
#include "AssetManager.h"
#include "Molecule.h"
#include "MoleculeManager.h"

void CellManager::Update()
{
	for (const auto& c : GetCells())
	{
		if (c->m_destroyFlag) continue;

		// ÚG‚µ‚½Molecule‚ÌŽæ‚èž‚Ý
		for (const auto& m : g_moleculeManagerPtr->m_indexer.GetNearParticles(c->m_position, c->m_radius * 2.0))
		{
			auto length = (m->m_position - c->m_position).length();

			if (!m->m_destroyFlag && length - m->m_radius - c->m_radius < 0.0) c->TakeMolecule(m);
		}

		// —]è‚ÌMolecule‚Ì“ŠŠü
		for (auto it = c->m_storage.m_molecules.begin(); it != c->m_storage.m_molecules.end(); ++it)
		{
			if ((*it).second > 10)
			{
				c->ExpireMolecule((*it).first);
				break;
			}
		}

		// Amino Acid‚Ì‡¬
		if (c->m_storage.NumMolecule("Amino Acid") < 5 &&
			c->m_storage.NumMolecule("Carbon") > 0 &&
			c->m_storage.NumMolecule("Oxygen") > 0 &&
			c->m_storage.NumMolecule("Nitrogen") > 0)
		{
			c->m_storage.AddMolecule(g_assetManagerPtr->GetMoleculeModel("Amino Acid"));
			c->m_storage.PullMolecule(g_assetManagerPtr->GetMoleculeModel("Carbon"));
			c->m_storage.PullMolecule(g_assetManagerPtr->GetMoleculeModel("Oxygen"));
			c->m_storage.PullMolecule(g_assetManagerPtr->GetMoleculeModel("Nitrogen"));
			c->m_molecules.AddMolecule(g_assetManagerPtr->GetMoleculeModel("Amino Acid"));
			c->m_molecules.PullMolecule(g_assetManagerPtr->GetMoleculeModel("Carbon"));
			c->m_molecules.PullMolecule(g_assetManagerPtr->GetMoleculeModel("Oxygen"));
			c->m_molecules.PullMolecule(g_assetManagerPtr->GetMoleculeModel("Nitrogen"));
		}

		// •ª—ôˆ—
		if (c->m_storage.NumMolecule("Amino Acid") >= 5 &&
			c->m_storage.NumMolecule("Carbon") >= 5 &&
			c->m_storage.NumMolecule("Oxygen") >= 5)
		{
			c->m_storage.PullMolecule(g_assetManagerPtr->GetMoleculeModel("Amino Acid"), 5);
			c->m_storage.PullMolecule(g_assetManagerPtr->GetMoleculeModel("Carbon"), 5);
			c->m_storage.PullMolecule(g_assetManagerPtr->GetMoleculeModel("Oxygen"), 5);
			c->m_molecules.PullMolecule(g_assetManagerPtr->GetMoleculeModel("Amino Acid"), 5);
			c->m_molecules.PullMolecule(g_assetManagerPtr->GetMoleculeModel("Carbon"), 5);
			c->m_molecules.PullMolecule(g_assetManagerPtr->GetMoleculeModel("Oxygen"), 5);
			c->RecalculatePhysicalProperty();

			const auto& nc = g_cellManagerPtr->AddCell();
			nc->m_position = c->m_position + Vector2D(1.0, 0.0).rotated(rand() / 360.0);
			nc->m_molecules.AddMolecule(g_assetManagerPtr->GetMoleculeModel("Amino Acid"), 5);
			nc->m_molecules.AddMolecule(g_assetManagerPtr->GetMoleculeModel("Carbon"), 5);
			nc->m_molecules.AddMolecule(g_assetManagerPtr->GetMoleculeModel("Oxygen"), 5);
			nc->RecalculatePhysicalProperty();
			nc->Init();
		}

		// Ž€–Sˆ—
		c->m_deathTimer -= g_fieldManagerPtr->GetDeltaTime();
		if (c->m_deathTimer <= 0.0)
		{
			// Molecule‚Ì“f‚«o‚µ
			for (const auto& m : c->m_molecules.m_molecules)
			{
				for (int i = 0; i < m.second; i++)
				{
					auto v = Vector2D(1.0, 0.0).rotated(rand() / 3600.0);
					auto& t = g_moleculeManagerPtr->AddMolecule(m.first);

					t->m_position = c->m_position + v * (rand() % 100) / 100.0 * (c->m_radius + t->m_radius);
					t->m_velocity = v * 2.0;
				}
			}

			c->m_destroyFlag = true;
		}
	}

	GetCells().erase(remove_if(GetCells().begin(), GetCells().end(), [](const auto& c) { return c->m_destroyFlag; }), GetCells().end());
	m_indexer.Update();
}
