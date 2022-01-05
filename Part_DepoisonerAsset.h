#pragma once

#include "PartAsset.h"

class Part_DepoisonerAsset : public PartAsset {
  double m_amount;

public:
  std::shared_ptr<PartState>
  makePartState(const std::shared_ptr<PartConfig>& partConfig) const override;

  // Get
  double getAmount() const { return m_amount; }

  // JSON
  void load(const JSON& json) override;
  void save(JSON& json) const override;

  bool isPreRenderOnStateEnabled() const override { return true; }
  bool isDrawOnStateEnabled() const override { return false; }

  // TypeName
  String getTypeName() override { return U"Part_DepoisonerAsset"; }
};
