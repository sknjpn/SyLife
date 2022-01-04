#include "Part_DepoisonerState.h"
#include "CellState.h"
#include "PartConfig.h"
#include "Part_DepoisonerAsset.h"
#include "World.h"


Part_DepoisonerState::Part_DepoisonerState(
    const std::shared_ptr<PartConfig> &partConfig)
    : PartState(partConfig),
      m_Part_DepoisonerAsset(std::dynamic_pointer_cast<Part_DepoisonerAsset>(
          partConfig->getPartAsset())) {}

void Part_DepoisonerState::update(CellState &cellState) {
  cellState.m_bioaccumulation =
      Max(0.0, cellState.m_bioaccumulation -
                   m_Part_DepoisonerAsset->getAmount() * DeltaTime);
}

void Part_DepoisonerState::load(Deserializer<BinaryReader> &reader) {}

void Part_DepoisonerState::save(Serializer<MemoryWriter> &writer) const {}
