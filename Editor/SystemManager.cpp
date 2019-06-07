#include "SystemManager.h"
#include "AssetManager.h"

unique_ptr<SystemManager>	g_systemManager;

SystemManager::SystemManager()
{
	g_assetManagerPtr = make_unique<AssetManager>();
}
