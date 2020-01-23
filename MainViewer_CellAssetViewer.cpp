#include "MainViewer.h"
#include "CellAsset.h"
#include "GUIText.h"
#include "GUITextBox.h"

MainViewer::CellAssetViewer::CellAssetViewer(const shared_ptr<CellAsset>& cellAsset)
	: m_cellAsset(cellAsset)
{
}

void MainViewer::CellAssetViewer::init()
{
	setViewerSize(800, 600);

	// name
	addChildViewer<GUIText>(m_cellAsset->getNameJP(), Font(24, Typeface::Heavy), GUIText::Mode::DrawLeftCenter)
		->setViewerRectInLocal(5, 5, 400, 40);
}

void MainViewer::CellAssetViewer::update()
{
	RectF(getViewerSize()).rounded(5.0).draw(ColorF(0.8)).drawFrame(2.0, 0.0, Palette::Black);

	// 数の推移
	{
		setDrawPos(5, 50);

		RectF(790, 80).draw(Color(11, 22, 33)).drawFrame(1.0, 0.0, Palette::Black);
	}

	// アイコン
	{
		setDrawPos(5, 140);
		const double size = 200;

		Circle(size / 2.0, size / 2.0, size / 2.0)
			.draw(ColorF(0.2))
			.drawFrame(2.0, 0.0, Palette::Black);

		auto t = Transformer2D(Mat3x2::Scale(0.75 * size / 2.0 / m_cellAsset->getRadius()).translated(size / 2.0, size / 2.0));
		m_cellAsset->draw();
	}

	// 性能
	{

	}
}
