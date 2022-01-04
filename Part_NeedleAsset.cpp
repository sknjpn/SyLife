#include "Part_NeedleAsset.h"
#include "Part_NeedleState.h"

std::shared_ptr<PartState> Part_NeedleAsset::makePartState(
    const std::shared_ptr<PartConfig> &partConfig) const {
  return MakeShared<Part_NeedleState>(partConfig);
}

void Part_NeedleAsset::load(const JSON &json) {
  PartAsset::load(json);

  m_penetrating = json[U"penetrating"].get<int>();
}

void Part_NeedleAsset::save(JSON &json) const {
  PartAsset::save(json);

  json.key(U"penetrating").writeDouble(m_penetrating);
}
