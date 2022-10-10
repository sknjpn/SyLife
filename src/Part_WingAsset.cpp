#include "Part_WingAsset.h"
#include "Part_WingState.h"

std::shared_ptr<PartState> Part_WingAsset::makePartState(const std::shared_ptr<PartConfig>& partConfig) const
{
  return std::make_shared<Part_WingState>(partConfig);
}

void Part_WingAsset::load(const JSON& json)
{
  PartAsset::load(json);

  m_isRight = json[U"isRight"].get<bool>();
  m_strength = json[U"strength"].get<double>();
}

void Part_WingAsset::save(JSON& json) const
{
  PartAsset::save(json);

  json[U"isRight"] = m_isRight;
  json[U"strength"] = m_strength;
}
