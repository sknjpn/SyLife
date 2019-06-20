#include "EquipmentConfig.h"

#include "AssetManager.h"

ptree EquipmentConfig::ToJSON() const
{
	ptree pt;

	// model
	pt.put("name", m_model->m_name);

	// position
	{
		ptree position;

		position.put("x", m_position.x);
		position.put("y", m_position.y);

		pt.push_back(std::make_pair("position", position));
	}

	// rotation
	pt.put("rotation", m_rotation);

	return pt;
}

void EquipmentConfig::FromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetEquipmentModel(pt.get<string>("name"));

	// position
	m_position = s3d::Vec2(pt.get_child("position").get<double>("x"), pt.get_child("position").get<double>("y"));

	// rotation
	m_rotation = pt.get<double>("rotation");
}
