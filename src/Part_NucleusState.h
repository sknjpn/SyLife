#pragma once

#include "PartState.h"

class Part_NucleusAsset;

class Part_NucleusState
  : public PartState
{
  std::shared_ptr<Part_NucleusAsset> m_Part_NucleusAsset;

public:
  Part_NucleusState(const std::shared_ptr<PartConfig>& partConfig);

  void update(CellState& cellState) override;
};
