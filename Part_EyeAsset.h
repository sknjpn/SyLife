#pragma once

#include "PartAsset.h"

class Part_EyeAsset : public PartAsset {
  double m_maxDistance;

public:
  // State
  std::shared_ptr<PartState>
  makePartState(const std::shared_ptr<PartConfig>& partConfig) const override;

  // JSON
  void load(const JSON& json) override;
  void save(JSON& json) const override;

  bool isPreRenderOnStateEnabled() const override { return true; }
  bool isDrawOnStateEnabled() const override { return false; }

  double getMaxDistance() const { return m_maxDistance; }

  // TypeName
  String getTypeName() override { return U"Part_EyeAsset"; }
};
