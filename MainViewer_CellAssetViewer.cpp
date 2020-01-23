#include "MainViewer.h"
#include "CellAsset.h"
#include "GUIText.h"
#include "GUITextBox.h"

MainViewer::CellAssetViewer::CellAssetViewer(const shared_ptr<CellAsset>& cellAsset)
{
}

void MainViewer::CellAssetViewer::init()
{
	setViewerSize(800, 600);

	// name
	addChildViewer<GUIText>(m_cellAsset->getNameJP(), Font(24, Typeface::Heavy), GUIText::Mode::DrawLeftCenter)
		->setViewerRectInLocal(5, 5, 128, 40);
}

void MainViewer::CellAssetViewer::update()
{
	RectF(getViewerSize()).rounded(5.0).draw(ColorF(1.0)).drawFrame(2.0, 0.0, Palette::Black);


	setDrawPos(5, 50);
	// 数の推移
	{
		RectF(790, 80).draw(Color(11, 22, 33)).drawFrame(1.0, 0.0, Palette::Black);
	}

	// アイコン
	{

	}

	// 性能
	{

	}
}
