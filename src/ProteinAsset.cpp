#include "ProteinAsset.h"
#include "ProteinAsset_Editor.h"

void ProteinAsset::load(const JSON& json) {
  Asset::load(json);

  // radius
  m_radius = json[U"radius"].get<double>();

  // color
  m_color = json[U"color"].get<Color>();

  // material
  m_material.load(json[U"material"]);
}

void ProteinAsset::save(JSON& json) const {
  Asset::save(json);

  // radius
  json[U"radius"] = m_radius;

  // color
  json[U"color"] = m_color;

  // material
  {
    JSON jsonMaterial;

    m_material.save(jsonMaterial);

    json[U"material"] = jsonMaterial;
  }
}

void ProteinAsset::makeEditor(const std::shared_ptr<EasyViewer>& parent) {
  parent->addChildViewer<ProteinAsset_Editor>(std::dynamic_pointer_cast<Asset>(shared_from_this()));
}
