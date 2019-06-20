#include "ModuleConfig.h"

#include "AssetManager.h"

ptree ModuleConfig::ToJSON() const
{
	return ptree();
}

void ModuleConfig::FromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetModuleModel(pt.get<string>("name"));
}
