#include "EquipmentConfig.h"

#include "AssetManager.h"

ptree EquipmentConfig::ToJSON() const
{
	return ptree();
}

void EquipmentConfig::FromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetEquipmentModel(pt.get<string>("name"));
}
