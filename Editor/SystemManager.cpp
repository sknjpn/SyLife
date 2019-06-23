#include "SystemManager.h"
#include "AssetManager.h"
#include "ViewerManager.h"
#include "Viewer.h"

unique_ptr<SystemManager>	g_systemManagerPtr;

SystemManager::SystemManager()
{
	g_assetManagerPtr = make_unique<AssetManager>();
	g_viewerManagerPtr = make_unique<ViewerManager>();

	g_assetManagerPtr->Init();
}

void SystemManager::Update()
{
	g_viewerManagerPtr->Update();
}

void SystemManager::Start()
{
	// ViewerManager
	for (const auto& v : g_viewerManagerPtr->m_viewers) v->Init();
}
