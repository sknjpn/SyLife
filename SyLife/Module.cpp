#include "Module.h"

#include "AssetManager.h"

void ModuleModel::SetFromJSON(const ptree & pt)
{
	PartModel::SetFromJSON(pt);
}

void ModuleConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetModel<ModuleModel>(pt.get<string>("name"));

	PartConfig::SetFromJSON(pt);
}
