#include "ModuleConfig.h"
#include "AssetManager.h"

ptree ModuleConfig::AddToJSON(ptree pt) const
{
	// type
	pt.put("type", "Module");

	return PartConfig::AddToJSON(pt);
}

void ModuleConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetModuleModel(pt.get<string>("name"));

	PartConfig::SetFromJSON(pt);
}
