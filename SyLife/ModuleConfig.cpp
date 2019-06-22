#include "ModuleConfig.h"
#include "AssetManager.h"

void ModuleConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetModel<ModuleModel>(pt.get<string>("name"));

	PartConfig::SetFromJSON(pt);
}