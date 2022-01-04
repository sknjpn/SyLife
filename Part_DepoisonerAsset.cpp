#include "Part_DepoisonerAsset.h"
#include "Part_DepoisonerState.h"

std::shared_ptr<PartState> Part_DepoisonerAsset::makePartState(
    const std::shared_ptr<PartConfig> &partConfig) const {
  return std::make_shared<Part_DepoisonerState>(partConfig);
}

void Part_DepoisonerAsset::load(const JSON &json) {
  PartAsset::load(json);

  m_amount = json[U"amount"].get<double>();
}

void Part_DepoisonerAsset::save(JSON &json) const {
  PartAsset::save(json);

  json[U"amount"] = m_amount;
}
