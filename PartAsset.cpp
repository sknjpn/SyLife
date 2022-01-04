#include "PartAsset.h"
#include "PartConfig.h"
#include "PartState.h"

void PartAsset::preRender(Image &image,
                          const std::shared_ptr<PartConfig> &partConfig) const {
  for (const auto &layer : m_shape)
    layer.m_polygon.rotated(partConfig->getRotation())
        .movedBy(partConfig->getPosition())
        .scaled(GeneralSetting::GetInstance().m_textureScale)
        .movedBy(Vec2(image.size()) / 2.0)
        .overwrite(image, ColorF(layer.m_color, 1.0));
}

void PartAsset::load(const JSON &json) {
  Asset::load(json);

  // mass
  m_mass = json[U"mass"].get<double>();

  // layers
  m_shape.load(json);

  // material
  m_material.load(json[U"material"]);
}

void PartAsset::save(JSON &json) const {
  Asset::save(json);

  // mass
  json[U"mass"] = m_mass;

  // layers
  m_shape.save(json);

  // material
  {
    JSON jsonMaterial;

    m_material.save(jsonMaterial);

    json[U"material"] = jsonMaterial;
  }
}
