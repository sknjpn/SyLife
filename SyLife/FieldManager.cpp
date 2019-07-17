#include "Cell.h"
#include "FieldManager.h"

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
}

void FieldManager::Update()
{
	g_terrainManagerPtr->Update();
	g_moleculeManagerPtr->Update();
	g_fieldManagerPtr->Update();
	g_waveManagerPtr->Update();
	g_cellManagerPtr->UpdateCellStates();
	g_eggManagerPtr->UpdateEggStates();
}

void FieldManager::Draw()
{
	g_terrainManagerPtr->Draw();
	g_waveManagerPtr->Draw();
	g_eggManagerPtr->DrawEggStates();
	g_moleculeManagerPtr->Draw();
	g_cellManagerPtr->DrawCellStates();
}
