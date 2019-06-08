#include "SystemManager.h"
#include "AssetManager.h"

unique_ptr<SystemManager>	g_systemManagerPtr;

SystemManager::SystemManager()
{
	g_assetManagerPtr = make_unique<AssetManager>();
}

void SystemManager::Update()
{

}