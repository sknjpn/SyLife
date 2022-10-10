#include "Part_SynthesizerState.h"
#include "CellAsset.h"
#include "CellState.h"
#include "PartConfig.h"
#include "Part_SynthesizerAsset.h"
#include "ProteinAsset.h"

Part_SynthesizerState::Part_SynthesizerState(
    const std::shared_ptr<PartConfig>& partConfig)
  : PartState(partConfig)
  , m_Part_SynthesizerAsset(std::dynamic_pointer_cast<Part_SynthesizerAsset>(partConfig->getPartAsset()))
{
}

void Part_SynthesizerState::draw(const CellState& cellState) const
{
  auto t = Transformer2D(getPartConfig()->getMat3x2());

  const auto& shape = getPartConfig()->getPartAsset()->getShape();
  const double alpha = Math::Lerp(0.1, 0.5, Min(1.0, m_timer / m_Part_SynthesizerAsset->getProductTime()));

  shape.getPreRenderTexture()
    .scaled(1.0 / GeneralSetting::GetInstance().m_textureScale)
    .draw(shape.getBoundingRect().pos, ColorF(1.0, alpha));
}

void Part_SynthesizerState::update(CellState& cellState)
{
  m_timer += DeltaTime;

  if (m_timer > m_Part_SynthesizerAsset->getProductTime() && cellState.m_storage >= m_Part_SynthesizerAsset->getExport()->getMaterial() && cellState.m_cellAsset->getMaxStorage().numProtein(m_Part_SynthesizerAsset->getExport()) > cellState.m_storage.numProtein(m_Part_SynthesizerAsset->getExport()))
  {
    m_timer = 0.0;

    cellState.m_storage -= m_Part_SynthesizerAsset->getExport()->getMaterial();
    cellState.m_storage.addProtein(m_Part_SynthesizerAsset->getExport());
  }
}

void Part_SynthesizerState::load(Deserializer<BinaryReader>& reader)
{
  reader >> m_timer;
}

void Part_SynthesizerState::save(Serializer<MemoryWriter>& writer) const
{
  writer << m_timer;
}
