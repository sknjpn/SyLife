#include "CellAsset.h"
#include "GUISlider.h"
#include "MainViewer.h"
#include "World.h"

void MainViewer::CellBook::addItem(const std::shared_ptr<CellAsset>& cellAsset) {
  addChildViewer<Item>(cellAsset);
}

void MainViewer::CellBook::init() {
  setViewerSize(130, 800);
  setFirstPosInLocal(Scene::Width() - 150, 180);
  setSecondPosInLocal(Scene::Width(), 180);

  addChildViewer<GUISlider>(0)->setViewerRectInLocal(RectF(130 - 20, 0, 20, 800));

  for (const auto& cellAsset : World::GetAssets<CellAsset>())
    if (cellAsset->isValid()) addItem(cellAsset);
}

void MainViewer::CellBook::update() {
  HiddenViewer::update();

  const auto items = getChildViewers<Item>();

  getChildViewer<GUISlider>()->setHeight(items.size() * 100.0);

  RectF(getViewerSize()).rounded(5).draw(Palette::Gray).drawFrame(2.0, 0.0, Palette::Black);

  for (int i = 0; i < items.size(); ++i) {
    const auto& item = items[i];

    item->setViewerPosInLocal(5, 5 + 100 * i - getChildViewer<GUISlider>()->getDelta());
  }
}
