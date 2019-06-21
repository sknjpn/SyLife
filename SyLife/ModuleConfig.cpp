#include "ModuleConfig.h"
#include "AssetManager.h"

void ModuleConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetModuleModel(pt.get<string>("name"));

	PartConfig::SetFromJSON(pt);
}