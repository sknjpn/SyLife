#include "CellManager.h"
#include "Cell.h"
#include "FieldManager.h"
#include "Searcher.h"
#include "Molecule.h"
#include "MoleculeManager.h"
#include "Searcher.h"

unique_ptr<CellManager>	g_cellManagerPtr;

CellManager::CellManager()
{
	m_cells.reserve(10000);
}


CellManager::~CellManager()
{
}

const shared_ptr<Cell>& CellManager::AddCell()
{
	const auto& c = m_cells.emplace_back(make_shared<Cell>());

	g_fieldManagerPtr->m_rigidbodies.emplace_back(c);
	m_indexer.AddParticle(c);

	return c;
}

void CellManager::Update()
{
	for (const auto& c : m_cells)
	{
		if (c->m_destroyFlag) continue;

		// ÚG‚µ‚½Molecule‚ÌŽæ‚èž‚Ý
		for (const auto& l : g_moleculeManagerPtr->m_indexer.GetNearParticles(c->m_position, c->m_radius * 2.0))
		{
			auto m = g_moleculeManagerPtr->m_indexer.GetParticles()[l.first];
			auto length = (m->m_position - c->m_position).length();

			if (!m->m_destroyFlag && m != c && length - m->m_radius - c->m_radius < 0.0) c->TakeMolecule(m);
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

		// Amino acid‚Ì‡¬
		if (c->m_storage.NumMolecule("Amino acid") < 5 &&
			c->m_storage.NumMolecule("Carbon") > 0 &&
			c->m_storage.NumMolecule("Oxygen") > 0 &&
			c->m_storage.NumMolecule("Nitrogen") > 0)
		{
			c->m_storage.AddMolecule(g_moleculeManagerPtr->GetModel("Amino acid"));
			c->m_storage.PullMolecule(g_moleculeManagerPtr->GetModel("Carbon"));
			c->m_storage.PullMolecule(g_moleculeManagerPtr->GetModel("Oxygen"));
			c->m_storage.PullMolecule(g_moleculeManagerPtr->GetModel("Nitrogen"));
			c->m_molecules.AddMolecule(g_moleculeManagerPtr->GetModel("Amino acid"));
			c->m_molecules.PullMolecule(g_moleculeManagerPtr->GetModel("Carbon"));
			c->m_molecules.PullMolecule(g_moleculeManagerPtr->GetModel("Oxygen"));
			c->m_molecules.PullMolecule(g_moleculeManagerPtr->GetModel("Nitrogen"));
		}

		// •ª—ôˆ—
		if (c->m_storage.NumMolecule("Amino acid") >= 5 &&
			c->m_storage.NumMolecule("Carbon") >= 5 &&
			c->m_storage.NumMolecule("Oxygen") >= 5)
		{
			c->m_storage.PullMolecule(g_moleculeManagerPtr->GetModel("Amino acid"), 5);
			c->m_storage.PullMolecule(g_moleculeManagerPtr->GetModel("Carbon"), 5);
			c->m_storage.PullMolecule(g_moleculeManagerPtr->GetModel("Oxygen"), 5);
			c->m_molecules.PullMolecule(g_moleculeManagerPtr->GetModel("Amino acid"), 5);
			c->m_molecules.PullMolecule(g_moleculeManagerPtr->GetModel("Carbon"), 5);
			c->m_molecules.PullMolecule(g_moleculeManagerPtr->GetModel("Oxygen"), 5);
			c->RecalculatePhysicalProperty();

			const auto& nc = g_cellManagerPtr->AddCell();
			nc->m_position = c->m_position + Vector2D(1.0, 0.0).rotated(rand() / 360.0);
			nc->m_molecules.AddMolecule(g_moleculeManagerPtr->GetModel("Amino acid"), 5);
			nc->m_molecules.AddMolecule(g_moleculeManagerPtr->GetModel("Carbon"), 5);
			nc->m_molecules.AddMolecule(g_moleculeManagerPtr->GetModel("Oxygen"), 5);
			nc->RecalculatePhysicalProperty();
			nc->Init();
		}

		// Ž€–Sˆ—
		c->m_deathTimer -= g_fieldManagerPtr->GetDeltaTime();
		if (c->m_deathTimer <= 0.0)
		{
			// Molecule‚Ì“f‚«o‚µ
			for (const auto& m : c->m_molecules.m_molecules) 
				for (int i = 0; i < m.second; i++) 
					g_moleculeManagerPtr->AddMolecule(m.first)->m_position = c->m_position + Vector2D((rand() % 100) / 100.0 * c->m_radius, 0.0).rotated(rand() / 3600.0);

			c->m_destroyFlag = true;
		}
	}

	m_cells.erase(remove_if(m_cells.begin(), m_cells.end(), [](const auto& c) { return c->m_destroyFlag; }), m_cells.end());
}
