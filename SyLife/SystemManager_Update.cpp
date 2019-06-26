#include "SystemManager.h"
#include "FieldManager.h"
#include "ViewerManager.h"

void SystemManager::Update()
{
	g_viewerManagerPtr->Update();
	/*
	auto f = s3d::KeySpace.pressed() ? 5 : 1;

	for (int i = 0; i < f; ++i)
		g_fieldManagerPtr->Update();
	*/
}
