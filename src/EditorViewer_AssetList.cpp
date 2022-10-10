﻿#include "Asset.h"
#include "EditorViewer.h"
#include "GUIButton.h"
#include "GUISlider.h"
#include "GUIText.h"
#include "World.h"

void EditorViewer::AssetList::init() {
  setViewerRectInLocal(Scene::Width() - 200.0, 0.0, 200.0, Scene::Height());
  addChildViewer<GUISlider>(0.0)->setViewerRectInLocal(RectF(180.0, 0, 20.0, Scene::Height()));
}

void EditorViewer::AssetList::update() {
  RectF(getViewerSize()).rounded(5.0).draw(ColorF(1.0)).drawFrame(2.0, 0.0, ColorF(0.0));

  const auto& items = getChildViewers<GUIButton>();
  getChildViewer<GUISlider>()->setHeight(items.size() * 25.0);

  // 再配置
  for (auto it = items.begin(); it != items.end(); ++it) {
    const auto& item = *it;

    item->setViewerPosInLocal(0, (it - items.begin()) * 25.0 - getChildViewer<GUISlider>()->getDelta());
  }

  // 入力
  for (auto it = items.begin(); it != items.end(); ++it) {
    const auto& item = *it;

    if (item->isSelected()) {
      const auto& asset = m_assets[it - items.begin()];

      getParentViewer<EditorViewer>()->openAsset(asset);
    }
  }
}

void EditorViewer::AssetList::addAsset(const std::shared_ptr<Asset>& asset) {
  m_assets.emplace_back(asset);

  const Vec2 size(180.0, 25.0);

  addChildViewer<GUIButton>()
      ->setViewerSize(size)
      ->addChildViewer<GUIText>(asset->getName(), Font(12, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
      ->setViewerRectInLocal(RectF(size).stretched(-6));

  // addChildViewer<Item>(asset);
}
