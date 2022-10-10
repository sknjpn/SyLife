#include "CellState.h"
#include "MainViewer.h"

MainViewer::FieldViewer::CellStateCaptureViewer::CellStateCaptureViewer(const std::shared_ptr<CellState>& cellState)
  : m_cellState(cellState)
{
}

void MainViewer::FieldViewer::CellStateCaptureViewer::init()
{
  setIsPenetrated(true);
}

void MainViewer::FieldViewer::CellStateCaptureViewer::update()
{
  if (MouseL.up() || m_cellState->isDestroyed())
  {
    destroy();
    return;
  }

  if (isMouseover()) getParentViewer<FieldViewer>()->getCamera().update();

  {
    // CellStateを移動する
    const auto t = getParentViewer<FieldViewer>()->getCamera().createTransformer();

    m_cellState->setVelocity((m_cellState->getPosition().lerp(Cursor::PosF(), 0.25) - m_cellState->getPosition()) / DeltaTime);
  }
}
