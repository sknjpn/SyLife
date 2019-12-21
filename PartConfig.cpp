#include "PartConfig.h"
#include "PartAsset.h"
#include "World.h"


Vec2 PartConfig::getCentroid() const
{
	return  m_position + m_partAsset->getShape().getCentroid().rotated(m_rotation);
}

void PartConfig::load(const JSONValue& json)
{
	Model::load(json);

	// asset
	m_partAsset = Assets::GetAsset<PartAsset>(json[U"asset"].getString());

	// position
	m_position = json[U"position"].get<Vec2>();

	// rotation
	m_rotation = json[U"rotation"].get<double>();
}

void PartConfig::save(JSONWriter& json) const
{
	Model::save(json);

	// asset
	json.key(U"asset").write(m_partAsset->getName());

	// position
	json.key(U"position").write(m_position);

	// rotation
	json.key(U"rotation").write(m_rotation);
}