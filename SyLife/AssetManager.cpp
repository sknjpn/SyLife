#include "AssetManager.h"

unique_ptr<AssetManager>	g_assetManagerPtr;

void AssetManager::Init()
{
	LoadMoleculeModels();
	LoadPartModels();
	LoadCellModels();
}
