#include "PartConfig.h"
#include "PartAsset.h"

#include "AssetManager.h"

Vec2 PartConfig::getCentroid() const
{
	return  m_position + m_partAsset->getShape().getCentroid().rotated(m_rotation);
}

void PartConfig::load(const ptree& pt)
{
	Model::load(pt);

	// asset
	m_partAsset = g_assetManagerPtr->getAsset<PartAsset>(pt.get<string>("asset"));

	// position
	m_position = Vec2(pt.get<double>("position.x"), pt.get<double>("position.y"));

	// rotation
	m_rotation = pt.get<double>("rotation");
}

void PartConfig::save(ptree& pt) const
{
	Model::save(pt);

	// asset
	pt.put("asset", m_partAsset->getName());

	// position
	{
		ptree position;

		position.put("x", m_position.x);
		position.put("y", m_position.y);

		pt.push_back(std::make_pair("position", position));
	}

	// rotation
	pt.put("rotation", m_rotation);

	pt.put("type", "PartConfig");
}