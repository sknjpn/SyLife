#include "SystemManager.h"
#include "AssetManager.h"
#include "ViewerManager.h"
#include "FileManager.h"

unique_ptr<SystemManager>	g_systemManagerPtr;

SystemManager::SystemManager()
{
	g_assetManagerPtr = make_unique<AssetManager>();
	g_viewerManagerPtr = make_unique<ViewerManager>();
	g_fileManagerPtr = make_unique<FileManager>();
}

void SystemManager::Update()
{
	g_viewerManagerPtr->Update();
}