#include "Part_EyeAsset.h"
#include "Part_EyeState.h"

std::shared_ptr<PartState> Part_EyeAsset::makePartState(
    const std::shared_ptr<PartConfig> &partConfig) const {
  return MakeShared<Part_EyeState>(partConfig);
}

void Part_EyeAsset::load(const JSON &json) {
  PartAsset::load(json);

  m_maxDistance = json[U"maxDistance"].get<double>();
}

void Part_EyeAsset::save(JSON &json) const {
  PartAsset::save(json);

  json.key(U"maxDistance").writeDouble(m_maxDistance);
}
