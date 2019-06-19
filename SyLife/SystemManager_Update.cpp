#include "SystemManager.h"
#include "FieldManager.h"

void SystemManager::Update()
{
	auto f = s3d::KeyShift.pressed() ? 10 : 5;
	for (int i = 0; i < f; ++i)
		g_fieldManagerPtr->Update();
}
