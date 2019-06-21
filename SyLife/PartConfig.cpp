#include "PartConfig.h"
#include "AssetManager.h"

void PartConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetPartModel(pt.get<string>("name"));

	// position
	m_position = Vector2D(pt.get<double>("position.x"), pt.get<double>("position.y"));

	// rotation
	m_rotation = pt.get<double>("rotation");

	Model::SetFromJSON(pt);
}