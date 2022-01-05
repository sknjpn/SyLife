#pragma once

#include "PartAsset.h"

class Part_WingAsset : public PartAsset {
  bool   m_isRight;
  double m_strength;

public:
  std::shared_ptr<PartState>
  makePartState(const std::shared_ptr<PartConfig>& partConfig) const override;

  // Get
  bool   getIsRight() const { return m_isRight; }
  double getStrength() const { return m_strength; }

  // JSON
  void load(const JSON& json) override;
  void save(JSON& json) const override;

  bool isPreRenderOnStateEnabled() const override { return false; }
  bool isDrawOnStateEnabled() const override { return true; }

  // TypeName
  String getTypeName() override { return U"Part_WingAsset"; }
};
