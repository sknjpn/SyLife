#include "EquipmentConfig.h"
#include "AssetManager.h"

ptree EquipmentConfig::AddToJSON(ptree pt) const
{
	// type
	pt.put("type", "Equipment");

	return PartConfig::AddToJSON(pt);
}

void EquipmentConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetModel<EquipmentModel>(pt.get<string>("name"));

	PartConfig::SetFromJSON(pt);
}