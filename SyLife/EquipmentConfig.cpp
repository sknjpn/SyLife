#include "EquipmentConfig.h"
#include "AssetManager.h"

void EquipmentConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetEquipmentModel(pt.get<string>("name"));

	PartConfig::SetFromJSON(pt);
}