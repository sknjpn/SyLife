#include "FieldManager.h"
#include "MoleculeManager.h"
#include "CellManager.h"
#include "Cell.h"

unique_ptr<FieldManager>	g_fieldManagerPtr;

void FieldManager::Init()
{
	ptree pt;
	read_json("assets/generation.json", pt);

	// molecules
	{
		for (auto molecules : pt.get_child("molecules"))
		{
			string name = molecules.second.get<string>("name");
			int size = molecules.second.get<int>("size");

			g_moleculeManagerPtr->AddMoleculesRandom(g_assetManagerPtr->GetModel<MoleculeModel>(name), size);
		}
	}

	//cells
	/*
	{
		for (auto cells : pt.get_child("cells"))
		{
			string name = cells.second.get<string>("name");
			int size = cells.second.get<int>("size");

			for (int i = 0; i < size; i++)
			{
				const auto& c = g_cellManagerPtr->AddCellState(g_assetManagerPtr->GetModel<CellModel>(name));
				c->SetPosition(Vec2(Random(m_size.x), Random(m_size.y)));
				c->SetVelocity(Vec2::Zero());

				c->Init();
			}
		}
	}
	*/
}

void FieldManager::Update()
{
	for (const auto& r : m_indexer.GetParticles())
		if (!r->IsDestroyed()) r->UpdateRigidbody();

	m_indexer.Update();
}
