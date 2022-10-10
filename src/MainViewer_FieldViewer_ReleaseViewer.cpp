#include "CellAsset.h"
#include "CellState.h"
#include "MainViewer.h"
#include "PartAsset.h"
#include "PartConfig.h"
#include "World.h"

MainViewer::FieldViewer::ReleaseViewer::ReleaseViewer(const std::shared_ptr<CellAsset>& cellAsset)
    : m_cellAsset(cellAsset) {
}

void MainViewer::FieldViewer::ReleaseViewer::init() {
  setIsPenetrated(true);
  setBackgroundColor(Color(0, 0));
}

void MainViewer::FieldViewer::ReleaseViewer::update() {
  {
    auto t1 = getParentViewer<FieldViewer>()->getCamera().createTransformer();

    if (!m_isInited) {
      m_isInited = true;
      m_rotation = 0.0;
      m_position = Cursor::PosF();
    }

    m_position = m_position.lerp(Cursor::PosF(), 0.25);

    if (Cursor::PosF() != m_position)
      m_rotation = -(Cursor::PosF() - m_position).getAngle(Vec2::Up());

    Circle(m_position, m_cellAsset->getRadius() * 1.5)
        .draw(ColorF(Palette::Orange, 0.5))
        .drawFrame(3.0, Palette::Black);

    {
      auto t2 = Transformer2D(Mat3x2::Scale(0.5).rotated(m_rotation).translated(m_position));

      m_cellAsset->getCellAssetTexture()
          .scaled(1.0 / GeneralSetting::GetInstance().m_textureScale)
          .drawAt(Vec2::Zero(), ColorF(1.0, 0.5));
    }
  }

  // Release
  if (MouseL.up()) {
    if (getParentViewer()->isMouseover() || !Scene::Rect().mouseOver()) {
      auto t1 = getParentViewer<FieldViewer>()->getCamera().createTransformer();

      // CellAssetのリセット
      m_cellAsset->setCentroidAsOrigin();

      // 新規Cell
      const auto& c = World::GetInstance()->addCellState(m_cellAsset);
      c->setPosition(m_position);
      c->setRotation(m_rotation);
      c->setVelocity((m_position.lerp(Cursor::PosF(), 0.25) - m_position) / DeltaTime);
      c->init();
    }

    destroy();
  }
}
