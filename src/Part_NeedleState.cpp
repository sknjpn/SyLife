#include "Part_NeedleState.h"
#include "CellAsset.h"
#include "CellState.h"
#include "PartConfig.h"
#include "Part_NeedleAsset.h"
#include "Part_NucleusAsset.h"
#include "World.h"

Part_NeedleState::Part_NeedleState(
    const std::shared_ptr<PartConfig>& partConfig)
  : PartState(partConfig)
  , m_Part_NeedleAsset(std::dynamic_pointer_cast<Part_NeedleAsset>(
    partConfig->getPartAsset()))
{
}

void Part_NeedleState::draw(const CellState& cellState) const
{
  auto t1 = Transformer2D(getPartConfig()->getMat3x2());
  auto t2 = Transformer2D(Mat3x2::Scale(1.0, Max(m_heat - 4.0, 0.0) * 1.0 + 1.0));

  const auto& shape = getPartConfig()->getPartAsset()->getShape();

  shape.getPreRenderTexture()
    .scaled(1.0 / GeneralSetting::GetInstance().m_textureScale)
    .draw(shape.getBoundingRect().pos, ColorF(1.0, Max(m_heat - 4.0, 0.0) * 0.9 + 0.1));
}

void Part_NeedleState::update(CellState& cellState)
{
  m_heat -= DeltaTime;

  if (m_heat < 0)
  {
    if (!cellState.isNeedNutrition())
    {
      m_heat = 1.0;

      return;
    }

    m_heat = 5.0;

    auto p = cellState.getWorldPosition(
        getPartConfig()->getPosition() + Vec2::Up().rotated(getPartConfig()->getRotation()) * 50.0);

    for (auto i : World::GetInstance()->getCellStateKDTree().knnSearch(2, p))
    {
      auto& t = World::GetInstance()->getCellStates()[i];

      if (!t->isDestroyed() && t->getRadius() > (t->getPosition() - p).length() && t->m_cellAsset != cellState.m_cellAsset)
      {
        // armor判定
        if (t->getCellAsset()->getNucleusAsset()->getArmor() <= m_Part_NeedleAsset->getPenetrating())
        {
          t->destroy();
          cellState.m_storage += t->m_storage;
          cellState.m_storage += t->m_cellAsset->getMaterial();
          cellState.m_bioaccumulation += t->m_bioaccumulation;
        }
      }
    }
  }
}

void Part_NeedleState::load(Deserializer<BinaryReader>& reader)
{
  reader >> m_heat;
}

void Part_NeedleState::save(Serializer<MemoryWriter>& writer) const
{
  writer << m_heat;
}
