#include "Part_PoisonerAsset.h"
#include "Part_PoisonerState.h"

std::shared_ptr<PartState> Part_PoisonerAsset::makePartState(
    const std::shared_ptr<PartConfig> &partConfig) const {
  return MakeShared<Part_PoisonerState>(partConfig);
}

void Part_PoisonerAsset::load(const JSON &json) {
  PartAsset::load(json);

  m_amount = json[U"amount"].get<double>();
  m_interval = json[U"interval"].get<double>();
}

void Part_PoisonerAsset::save(JSON &json) const {
  PartAsset::save(json);

  json.key(U"amount").writeDouble(m_amount);
  json.key(U"interval").writeDouble(m_interval);
}
