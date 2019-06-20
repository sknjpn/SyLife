#include "ModuleConfig.h"

#include "AssetManager.h"

void ModuleConfig::FromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetModuleModel(pt.get<string>("name"));

	// position
	m_position = s3d::Vec2(pt.get_child("position").get<double>("x"), pt.get_child("position").get<double>("y"));

	// rotation
	m_rotation = pt.get<double>("rotation");
}
