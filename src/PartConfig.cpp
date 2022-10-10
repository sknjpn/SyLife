#include "PartConfig.h"
#include "PartAsset.h"
#include "World.h"

Vec2 PartConfig::getCentroid() const
{
  return m_position + m_partAsset->getShape().getCentroid().rotated(m_rotation);
}

void PartConfig::load(const JSON& json)
{
  Object::load(json);

  // asset
  m_partAsset = World::GetAsset<PartAsset>(json[U"asset"].getString());

  // position
  m_position = json[U"position"].get<Vec2>();

  // rotation
  m_rotation = json[U"rotation"].get<double>();
}

void PartConfig::save(JSON& json) const
{
  Object::save(json);

  // asset
  json[U"asset"] = m_partAsset->getName();

  // position
  json[U"position"] = m_position;

  // rotation
  json[U"rotation"] = m_rotation;
}
