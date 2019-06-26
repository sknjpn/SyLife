#include "SystemManager.h"
#include "FieldManager.h"
#include "ViewerManager.h"

void SystemManager::Update()
{
	g_viewerManagerPtr->Update();
}
