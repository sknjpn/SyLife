#include "FieldManager.h"

unique_ptr<FieldManager>	g_fieldManagerPtr;

void FieldManager::Init()
{
	LoadGenerationSetting();
}
