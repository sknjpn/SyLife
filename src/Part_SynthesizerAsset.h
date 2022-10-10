#pragma once

#include "PartAsset.h"

class Part_SynthesizerAsset : public PartAsset
{
  std::shared_ptr<ProteinAsset> m_export;

  double m_productTime;

public:
  // State
  std::shared_ptr<PartState> makePartState(const std::shared_ptr<PartConfig>& partConfig) const override;

  // Get
  const std::shared_ptr<ProteinAsset>& getExport() const { return m_export; }

  double getProductTime() const { return m_productTime; }

  // JSON
  void load(const JSON& json) override;
  void save(JSON& json) const override;

  bool isPreRenderOnStateEnabled() const override { return false; }
  bool isDrawOnStateEnabled() const override { return true; }

  // TypeName
  String getTypeName() override { return U"Part_SynthesizerAsset"; }
};
