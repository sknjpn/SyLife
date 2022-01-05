#include "Part_SynthesizerAsset.h"
#include "Part_SynthesizerState.h"
#include "ProteinAsset.h"
#include "World.h"

std::shared_ptr<PartState> Part_SynthesizerAsset::makePartState(
    const std::shared_ptr<PartConfig>& partConfig) const {
  return std::make_shared<Part_SynthesizerState>(partConfig);
}

void Part_SynthesizerAsset::load(const JSON& json) {
  PartAsset::load(json);

  m_export = World::GetAsset<ProteinAsset>(json[U"export"].getString());
  m_productTime = json[U"productTime"].get<double>();
}

void Part_SynthesizerAsset::save(JSON& json) const {
  PartAsset::save(json);

  json[U"export"] = m_export->getName();
  json[U"productTime"] = m_productTime;
}
