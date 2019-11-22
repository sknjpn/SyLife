#include "GUIAssetLinker.h"
#include "Asset.h"

void GUIAssetLinker::init()
{

}

void GUIAssetLinker::update()
{
	RectF(getViewerSize()).rounded(5)
		.draw(Palette::White)
		.drawFrame(2.0, 0.0, Palette::Black);

	{
		const auto t = Transformer2D(Mat3x2::Scale(Min(getViewerSize().x, getViewerSize().y)));

		m_asset->drawIcon();
	}

	if (const auto dragger = GetRootViewer()->getChildViewer<Dragger>())
	{
		if (MouseL.up() && RectF(getViewerSize()).rounded(5).mouseOver())
		{
			m_asset = dragger->getAsset();
			m_functionOnChanged(dragger->getAsset());
		}

		RectF(getViewerSize()).rounded(5)
			.draw(ColorF(0.5, 0.5))
			.drawFrame(2.0, 0.0, Palette::Black);
	}
}
