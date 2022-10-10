﻿#pragma once

#include "PartState.h"

class Part_EyeAsset;

class Part_EyeState
    : public PartState {
  std::shared_ptr<Part_EyeAsset> m_Part_EyeAsset;
  std::shared_ptr<CellState>     m_targetCellState;

  double m_heat = 0.0;

public:
  Part_EyeState(const std::shared_ptr<PartConfig>& partConfig);

  const std::shared_ptr<Part_EyeAsset>& getPart_EyeAsset() const { return m_Part_EyeAsset; }
  const std::shared_ptr<CellState>&     getTargetCellState() const { return m_targetCellState; }

  void update(CellState& cellState) override;
};
