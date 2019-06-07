#include "SystemManager.h"
#include "FieldManager.h"
#include "AssetManager.h"
#include "MoleculeManager.h"
#include "CellManager.h"
#include "RecipeManager.h"
#include "PartManager.h"

unique_ptr<SystemManager>	g_systemManagerPtr;

SystemManager::SystemManager()
{
	g_assetManagerPtr = make_unique<AssetManager>();
	g_cellManagerPtr = make_unique<CellManager>();
	g_fieldManagerPtr = make_unique<FieldManager>();
	g_moleculeManagerPtr = make_unique<MoleculeManager>();
	g_partManagerPtr = make_unique<PartManager>();
	g_recipeManagerPtr = make_unique<RecipeManager>();

	g_assetManagerPtr->Init();
	g_moleculeManagerPtr->Init();
	g_partManagerPtr->Init();
	g_fieldManagerPtr->Init();
}
