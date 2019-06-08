#include "SystemManager.h"
#include "FieldManager.h"

void SystemManager::Update()
{
	g_fieldManagerPtr->Update();
}
