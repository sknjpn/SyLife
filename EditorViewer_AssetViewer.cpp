#include "Asset.h"
#include "EditorViewer.h"

void EditorViewer::AssetViewer::init() {
  setViewerRectInLocal(200, 0, Scene::Width() - 200 - 200, Scene::Height());

  m_asset->makeEditor(shared_from_this());
}

void EditorViewer::AssetViewer::update() {
  RectF(getViewerSize()).rounded(5.0).draw(ColorF(1.0)).drawFrame(2.0, 0.0, ColorF(0.0));
}
