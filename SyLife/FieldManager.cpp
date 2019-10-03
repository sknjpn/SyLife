#include "Cell.h"
#include "FieldManager.h"
#include "Chip.h"

unique_ptr<FieldManager>	g_fieldManagerPtr;

void FieldManager::Init()
{
	// Chips
	{
		const Size size(128, 128);

		for (auto p : step(size))
			m_chips[p] = MakeShared<Chip>();

		// 結びつきの作成
		for (auto p : step(size.movedBy(-1, -1)))
		{
			m_chips[p]->m_r = m_chips[p.movedBy(1, 0)];
			m_chips[p]->m_d = m_chips[p.movedBy(0, 1)];
			m_chips[p.movedBy(1, 0)]->m_l = m_chips[p];
			m_chips[p.movedBy(0, 1)]->m_u = m_chips[p];
		}
	}

	ptree pt;
	read_json("assets/generation.json", pt);

	// molecules
	/*
	{
		for (auto molecules : pt.get_child("molecules"))
		{
			string name = molecules.second.get<string>("name");
			int size = molecules.second.get<int>("size");

			g_moleculeManagerPtr->AddMoleculesRandom(g_assetManagerPtr->GetModel<MoleculeModel>(name), size);
		}
	}*/
}
