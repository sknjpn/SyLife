#include "EditorViewer.h"
#include "Asset.h"
#include "GUIText.h"

void EditorViewer::AssetList::Item::init()
{
}

void EditorViewer::AssetList::Item::update()
{
	RectF(getViewerSize()).stretched(-2.0).rounded(5.0).draw(ColorF(1.0)).drawFrame(2.0, 0.0, ColorF(0.0));

	// Nameの描画
	{

	}

	// Textureの描画
	{
		const auto region = RectF(getViewerSize()).stretched(-5);
		const auto& texture = m_asset->getAssetIcon();
		const auto scale = Min(region.w / texture.width(), region.h / texture.height());

		texture.scaled(scale).drawAt(region.center());
	}
}
