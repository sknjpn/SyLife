#include "GUIAssetLinker.h"
#include "Asset.h"

void GUIAssetLinker::Dragger::init()
{
	setViewerRectInLocal(RectF(50, 50).setCenter(Cursor::PosF()));
}

void GUIAssetLinker::Dragger::update()
{
	if (MouseL.up()) destroy();

	RectF(getViewerSize()).rounded(5).draw(ColorF(0.5, 0.5)).drawFrame(2.0, 0.0, Palette::Black);

	{
		const auto t = Transformer2D(Mat3x2::Scale(50));

		m_asset->drawIcon();
	}

	// 最後に位置は修正
	setViewerRectInLocal(RectF(50, 50).setCenter(getViewerPosInWorld() + Cursor::PosF()));
}
