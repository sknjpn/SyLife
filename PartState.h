#pragma once

#include "common.h"

class PartConfig;
class CellState;

class PartState {
  std::shared_ptr<PartConfig> m_partConfig;

public:
  PartState(const std::shared_ptr<PartConfig>& partConfig)
      : m_partConfig(partConfig) { }

  virtual ~PartState() { }

  // Get
  const std::shared_ptr<PartConfig>& getPartConfig() const {
    return m_partConfig;
  }

  virtual void draw(const CellState& cellState) const { }
  virtual void update(CellState& cellState) { }

  virtual void load(Deserializer<BinaryReader>& reader) { }
  virtual void save(Serializer<MemoryWriter>& writer) const { }
};
