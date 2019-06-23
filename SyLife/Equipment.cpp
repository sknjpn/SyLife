#include "Equipment.h"
#include "AssetManager.h"

void EquipmentModel::SetFromJSON(const ptree & pt)
{
	PartModel::SetFromJSON(pt);
}

void EquipmentConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetModel<EquipmentModel>(pt.get<string>("name"));

	PartConfig::SetFromJSON(pt);
}
