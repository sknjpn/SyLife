#pragma once

#include "PartState.h"

class Part_DepoisonerAsset;

class Part_DepoisonerState : public PartState
{
  std::shared_ptr<Part_DepoisonerAsset> m_Part_DepoisonerAsset;

public:
  Part_DepoisonerState(const std::shared_ptr<PartConfig>& partConfig);

  const std::shared_ptr<Part_DepoisonerAsset>& getPart_DepoisonerAsset() const
  {
    return m_Part_DepoisonerAsset;
  }

  void update(CellState& cellState) override;

  void load(Deserializer<BinaryReader>& reader) override;
  void save(Serializer<MemoryWriter>& writer) const override;
};
