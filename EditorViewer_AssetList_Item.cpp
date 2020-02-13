#include "EditorViewer.h"
#include "Asset.h"
#include "GUIText.h"
#include "GUITexture.h"

void EditorViewer::AssetList::Item::init()
{
	addChildViewer<GUIText>(m_asset->getName(), Font(18, Typeface::Bold), GUIText::Mode::DrawAtCenter, Palette::Black)
		->setViewerRectInLocal(120, 30);

	addChildViewer<GUITexture>(m_asset->getAssetIcon(), Palette::Black)
		->setViewerRectInLocal(0, 30, 120, 120);
}

void EditorViewer::AssetList::Item::update()
{
	RectF(getViewerSize()).stretched(-2.0).rounded(5.0).draw(ColorF(1.0)).drawFrame(2.0, 0.0, ColorF(0.0));
}
