#include "PartConfig.h"
#include "PartAsset.h"

#include "AssetManager.h"

double PartConfig::getInertia(const Vec2& center) const
{
	auto d = m_position + m_partModel->getShape().getCentroid().rotated(m_rotation) - center;

	auto is = m_partModel->getShape().getInertia(m_partModel->getMass());
	auto id = d.lengthSq() * m_partModel->getMass();

	return is + id;
}

void PartConfig::load_this(const ptree& pt)
{
	// asset
	m_partModel = g_assetManagerPtr->getAsset<PartAsset>(pt.get<string>("asset"));

	// position
	m_position = Vec2(pt.get<double>("position.x"), pt.get<double>("position.y"));

	// rotation
	m_rotation = pt.get<double>("rotation");

	Model::load_this(pt);
}

void PartConfig::save_this(ptree& pt) const
{
	// asset
	pt.put("asset", m_partModel->getName());

	// position
	{
		ptree position;

		position.put("x", m_position.x);
		position.put("y", m_position.y);

		pt.push_back(std::make_pair("position", position));
	}

	// rotation
	pt.put("rotation", m_rotation);

	Model::save_this(pt);

	pt.put("type", "PartConfig");
}