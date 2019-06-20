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

	// position
	m_position = s3d::Vec2(pt.get_child("position").get<double>("x"), pt.get_child("position").get<double>("y"));

	// rotaition
	m_rotation = pt.get<double>("rotation");
}
