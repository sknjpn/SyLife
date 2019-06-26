#include "SystemManager.h"
#include "ViewerManager.h"

void SystemManager::Update()
{
	g_viewerManagerPtr->Update();
}
