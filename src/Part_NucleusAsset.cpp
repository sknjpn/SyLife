﻿#include "Part_NucleusAsset.h"
#include "Part_NucleusState.h"

std::shared_ptr<PartState> Part_NucleusAsset::makePartState(
    const std::shared_ptr<PartConfig>& partConfig) const {
  return std::make_shared<Part_NucleusState>(partConfig);
}

void Part_NucleusAsset::load(const JSON& json) {
  PartAsset::load(json);

  m_lifespanTime = json[U"lifespanTime"].get<double>();
  m_bornTime = json[U"bornTime"].get<double>();
  m_yieldTime = json[U"yieldTime"].get<double>();

  m_armor = json[U"armor"].get<int>();
}

void Part_NucleusAsset::save(JSON& json) const {
  PartAsset::save(json);

  // time
  json[U"lifespanTime"] = m_lifespanTime;
  json[U"bornTime"] = m_bornTime;
  json[U"yieldTime"] = m_yieldTime;

  json[U"armor"] = m_armor;
}
