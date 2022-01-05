#pragma once

#include "PartAsset.h"

class Part_NeedleAsset : public PartAsset {
  int m_penetrating;

public:
  // State
  std::shared_ptr<PartState>
  makePartState(const std::shared_ptr<PartConfig>& partConfig) const override;

  // JSON
  void load(const JSON& json) override;
  void save(JSON& json) const override;

  bool isPreRenderOnStateEnabled() const override { return false; }
  bool isDrawOnStateEnabled() const override { return true; }

  int getPenetrating() const { return m_penetrating; }

  // TypeName
  String getTypeName() override { return U"Part_NeedleAsset"; }
};
