#include "PartConfig.h"
#include "PartAsset.h"

#include "AssetManager.h"

Vec2 PartConfig::getCentroid() const
{
	return  m_position + m_partAsset->getShape().getCentroid().rotated(m_rotation);
}

void PartConfig::load(const JSONValue& json)
{
	Model::load(json);

	// asset
	m_partAsset = g_assetManagerPtr->getAsset<PartAsset>(json[U"asset"].getString());

	// position
	m_position = json[U"position"].get<Vec2>();

	// rotation
	m_rotation = json[U"rotation"].get<double>();
}

void PartConfig::save(const JSONWriter& json) const
{
	Model::save(json);

	// asset
	//pt.put(U"asset", m_partAsset->getName());

	// position
	/*{
		ptree position;

		position.put(U"x", m_position.x);
		position.put(U"y", m_position.y);

		pt.push_back(std::make_pair(U"position", position));
	}*/

	// rotation
	//pt.put(U"rotation", m_rotation);

	//pt.put(U"type", "PartConfig");
}