#include "CellState.h"
#include "MainViewer.h"
#include "ProteinAsset.h"

MainViewer::CellAssetViewer::CellStateViewer::CellStateViewer(
    const std::shared_ptr<CellState>& cellState)
  : m_cellState(cellState)
{
}

void MainViewer::CellAssetViewer::CellStateViewer::init()
{
  setViewerRectInLocal(5, 345, 200, 250);
}

void MainViewer::CellAssetViewer::CellStateViewer::update()
{
  if (m_cellState->isDestroyed())
  {
    destroy();
    return;
  }

  RectF(getViewerSize())
    .rounded(5)
    .draw(Palette::White)
    .drawFrame(1.0, 0.0, Palette::Black);

  {
    moveDrawPos(5, 5);

    static Font font(16, Typeface::Regular);

    font(U"現在選択中の生き物の状態").draw(Vec2::Zero(), Palette::Black);
    moveDrawPos(0, 20);

    font(U"寿命:", Ceil(m_cellState->getCellAsset()->getLifespanTime() - m_cellState->m_startTimer), U"秒")
      .draw(Vec2::Zero(), Palette::Black);
    moveDrawPos(0, 20);

    font(U"持っている栄養").draw(Vec2::Zero(), Palette::Black);
    moveDrawPos(0, 20);
    {
      moveDrawPos(8, 0);

      font(U"エレメント:", int(m_cellState->m_storage.getElement()), U"elm")
        .draw(Vec2::Zero(), Palette::Black);
      moveDrawPos(0, 20);

      for (const auto& protein : m_cellState->m_storage.getProteinList())
      {
        font(protein.first->getNameJP() + U": " + ToString(protein.second) + U"個")
          .draw(Vec2::Zero(), Palette::Black);

        moveDrawPos(0, 20);
      }

      moveDrawPos(-8, 0);
    }

    if (m_cellState->m_storage.contain(m_cellState->getCellAsset()->getMaterial()))
    {
      font(U"産卵まで", Ceil(m_cellState->getCellAsset()->getYieldTime() - m_cellState->m_yieldTimer), U"秒")
        .draw(Vec2::Zero(), Palette::Black);
      moveDrawPos(0, 20);
    }
    else
    {
      font(U"産卵に必要な栄養").draw(Vec2::Zero(), Palette::Black);
      moveDrawPos(0, 20);
    }

    {
      moveDrawPos(8, 0);

      {
        int num = int(
            Max(0.0, m_cellState->getCellAsset()->getMaterial().getElement() - m_cellState->m_storage.getElement()));

        if (num > 0)
        {
          font(U"エレメント:", num, U"elm").draw(Vec2::Zero(), Palette::Red);
          moveDrawPos(0, 20);
        }
      }

      for (const auto& protein :
          m_cellState->getCellAsset()->getMaterial().getProteinList())
      {
        int num = Max(0, protein.second - m_cellState->m_storage.numProtein(protein.first));

        if (num > 0)
        {
          font(protein.first->getNameJP() + U": " + ToString(num) + U"個")
            .draw(Vec2::Zero(), Palette::Red);
          moveDrawPos(0, 20);
        }
      }

      moveDrawPos(-8, 0);
    }
  }
}
