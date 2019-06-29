#include "SystemManager.h"
#include "FieldManager.h"
#include "AssetManager.h"
#include "MoleculeManager.h"
#include "CellManager.h"
#include "EggManager.h"
#include "WaveManager.h"
#include "ViewerManager.h"
#include "Viewer.h"

unique_ptr<SystemManager>	g_systemManagerPtr;

SystemManager::SystemManager()
{
	g_assetManagerPtr = make_unique<AssetManager>();
	g_cellManagerPtr = make_unique<CellManager>();
	g_fieldManagerPtr = make_unique<FieldManager>();
	g_moleculeManagerPtr = make_unique<MoleculeManager>();
	g_viewerManagerPtr = make_unique<ViewerManager>();
	g_eggManagerPtr = make_unique<EggManager>();
	g_waveManagerPtr = make_unique<WaveManager>();

	g_assetManagerPtr->Init();
	g_fieldManagerPtr->Init();
}
