#include "PartConfig.h"
#include "AssetManager.h"

ptree PartConfig::AddToJSON(ptree pt) const
{
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

	return Model::AddToJSON(pt);
}

void PartConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetPartModel(pt.get<string>("name"));

	// position
	m_position = s3d::Vec2(pt.get<double>("position.x"), pt.get<double>("position.y"));

	// rotation
	m_rotation = pt.get<double>("rotation");

	Model::SetFromJSON(pt);
}