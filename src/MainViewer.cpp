#include "MainViewer.h"
#include "CellState.h"
#include "GUIButton.h"
#include "GUICurtain.h"
#include "GUIText.h"
#include "World.h"

void MainViewer::openCellMakingViewer()
{
  if (!hasChildViewer<CellMakingViewer>()) addChildViewer<CellMakingViewer>();
}

void MainViewer::setHiddenMode()
{
  m_hiddenMode = true;

  for (auto hv : getChildViewers<HiddenViewer>())
    hv->moveToSecondPos();

  if (auto viewer = getChildViewer<GUIButton>()) { viewer->destroy(); }
  if (auto viewer = getChildViewer<CellMakingViewer>()) { viewer->destroy(); }

  for (auto viewer : getChildViewers<CellAssetViewer>())
    viewer->destroy();

  getChildViewer<FieldViewer>()->m_handAction = FieldViewer::HandAction::None;
}

void MainViewer::unsetHiddenMode()
{
  m_hiddenMode = false;

  for (auto hv : getChildViewers<HiddenViewer>())
    hv->moveToFirstPos();

  addChildViewer<GUIButton>([this]() { openCellMakingViewer(); })
    ->setViewerRectInLocal(100, 50, 250, 50)
    ->addChildViewer<GUIText>(U"生き物作成", Font(32, Typeface::Bold));
}

void MainViewer::init()
{
  addChildViewer<FieldViewer>();

  addChildViewer<GUIButton>([this]() { openCellMakingViewer(); })
    ->setViewerRectInLocal(100, 50, 250, 50)
    ->addChildViewer<GUIText>(U"生き物作成", Font(32, Typeface::Bold));

  addChildViewer<CellBook>();

  addChildViewer<CommandPalette>();

  // OpenCurtain
  addChildViewer<GUICurtain>(Color(11, 22, 33), Color(0, 0), 0.5);

  if (GeneralSetting::GetInstance().m_autoTurnOutEnabled)
    m_uncontrolTimer.start();
}

void MainViewer::update()
{
  // HiddenModeの実行
  if (m_hiddenMode && MouseL.down())
  {
    m_uncontrolTimer.reset();

    unsetHiddenMode();
  }

  if (MouseL.up()) m_uncontrolTimer.restart();
  if (!m_hiddenMode && GeneralSetting::GetInstance().m_autoTurnOutEnabled && m_uncontrolTimer.s() > GeneralSetting::GetInstance().m_autoTurnOutTime) setHiddenMode();

  if (KeyP.down()) World::GetInstance()->save();
}

void MainViewer::onDestroy()
{
  if (World::GetInstance())
    World::GetInstance()->save();
}

void MainViewer::addCellAssetViewer(const std::shared_ptr<CellAsset>& cellAsset)
{
  if (!hasChildViewer(cellAsset->getName()))
  {
    addChildViewer<CellAssetViewer>(cellAsset)
      ->setViewerPosInLocal(Vec2(1.0, 1.0).setLength(50.0 * getChildViewers<CellAssetViewer>().size()));
  }
}

void MainViewer::addCellAssetViewer(const std::shared_ptr<CellState>& cellState)
{
  if (hasChildViewer(cellState->getCellAsset()->getName()))
  {
    getChildViewer<CellAssetViewer>(cellState->getCellAsset()->getName())
      ->setCellState(cellState);

    getChildViewer<CellAssetViewer>(cellState->getCellAsset()->getName())
      ->moveToFront();
  }
  else
  {
    addChildViewer<CellAssetViewer>(cellState)
      ->setViewerPosInLocal(Vec2(1.0, 1.0).setLength(50.0 * getChildViewers<CellAssetViewer>().size()));
  }
}
