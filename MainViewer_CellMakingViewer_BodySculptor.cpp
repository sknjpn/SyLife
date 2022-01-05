#include "CellAsset.h"
#include "GUIButton.h"
#include "GUIChecker.h"
#include "GUIText.h"
#include "GUIValuer.h"
#include "MainViewer.h"
#include "Part_BodyAsset.h"

void MainViewer::CellMakingViewer::BodySculptor::init() {
  setViewerRectInLocal(300, 0, 1200, 1100);

  const auto bodyAsset = getParentViewer<CellMakingViewer>()->getCellAsset()->getBodyAsset();
  // addChildViewer<LayerLists>(bodyAsset);
  addChildViewer<ColorSelector>();
  addChildViewer<Workspace>(bodyAsset);

  // 左右対象の設定
  {
    m_isSymmetrical = false;

    addChildViewer<GUIChecker>()
        ->setName(U"左右対称")
        ->setViewerRectInLocal(5, 5, 30, 30);

    addChildViewer<GUIText>(U"左右対称", Font(18, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
        ->setViewerRectInLocal(40, 5, 165, 30);

    addChildViewer<GUIText>(U"スタンプサイズ", Font(18, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
        ->setViewerRectInLocal(5, 170, 165, 30);

    addChildViewer<GUIValuer>(0.5)
        ->setName(U"スタンプサイズ")
        ->setViewerRectInLocal(5, 200, 190, 30);
  }
}

void MainViewer::CellMakingViewer::BodySculptor::update() {
}

double MainViewer::CellMakingViewer::BodySculptor::getStampRadius() const {
  return Math::Lerp(5.0, 20.0, getChildViewer<GUIValuer>(U"スタンプサイズ")->getValue());
}
