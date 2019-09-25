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
