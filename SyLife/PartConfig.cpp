#include "PartConfig.h"
#include "PartAsset.h"

#include "AssetManager.h"

double PartConfig::getInertia() const
{
	return m_partModel->getRectInertia() + (m_position + m_partModel->getCenter().rotated(m_rotation)).lengthSq() * m_partModel->getMass();
}

void PartConfig::load_this(const ptree& pt)
{
	// model
	m_partModel = g_assetManagerPtr->getAsset<PartAsset>(pt.get<string>("model"));

	// position
	m_position = Vec2(pt.get<double>("position.x"), pt.get<double>("position.y"));

	// rotation
	m_rotation = pt.get<double>("rotation");

	Model::load_this(pt);
}

void PartConfig::save_this(ptree& pt) const
{
	// model
	pt.put("model", m_partModel->getName());

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