#include "MainViewer.h"
#include "CellAsset.h"
#include "GUIText.h"
#include "GUITextBox.h"

MainViewer::CellAssetViewer::CellAssetViewer(const shared_ptr<CellAsset>& cellAsset)
{
}

void MainViewer::CellAssetViewer::init()
{
	addChildViewer<GUIText>(m_cellAsset->getNameJP(), Font(24, Typeface::Heavy), GUIText::)
		->setViewerRectInLocal(5, 5, 128, 40);
}

void MainViewer::CellAssetViewer::update()
{
	RectF(getViewerSize()).rounded(5.0).draw(ColorF(1.0)).drawFrame(2.0, 0.0, Palette::Black);

	// name
	{
		
	}

	// 数の推移
	{

	}

	// アイコン
	{

	}

	// 性能
	{

	}
}
