#pragma once

#include "Rigidbody.h"

class CellAsset;

class EggState : public Rigidbody {
  std::shared_ptr<CellAsset> m_cellAsset;

  double m_timer;

public:
  EggState(const std::shared_ptr<CellAsset>& cellAsset);
  EggState(Deserializer<BinaryReader>& reader) { load(reader); }

  const std::shared_ptr<CellAsset>& getCellAsset() const { return m_cellAsset; }

  double getTimer() const { return m_timer; }

  void updateEgg();
  void draw1();
  void draw2();

  void load(Deserializer<BinaryReader>& reader);
  void save(Serializer<MemoryWriter>& writer) const;
};
