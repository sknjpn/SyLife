#pragma once

#include "PartState.h"

class Part_BodyAsset;

class Part_BodyState
  : public PartState
{
  std::shared_ptr<Part_BodyAsset> m_Part_BodyAsset;

public:
  Part_BodyState(const std::shared_ptr<PartConfig>& partConfig);

  void update(CellState& cellState) override;
};
