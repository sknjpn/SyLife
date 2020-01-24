#include "MainViewer.h"
#include "CellAsset.h"

MainViewer::CellBook::Item::Item(const shared_ptr<CellAsset>& cellAsset)
	: m_cellAsset(cellAsset)
{}

void MainViewer::CellBook::Item::init()
{
	setViewerSize(100, 100);
}

void MainViewer::CellBook::Item::update()
{
	RectF(getViewerSize()).stretched(-5).draw(Palette::Black);

	{
		auto t = Transformer2D(Mat3x2::Scale(0.5 * 50 / m_cellAsset->getRadius()).translated(50, 50));

		m_cellAsset->draw();
	}

	RectF(getViewerSize()).rounded(5)
		.drawFrame(5.0, 0.0, Palette::White)
		.drawFrame(1.0, 0.0, Palette::Black);

	if (isMouseover())
	{
		RectF(getViewerSize()).rounded(5).draw(ColorF(1.0, 0.25));

		if (MouseL.down())
		{
			getParentViewer()->getParentViewer()->getChildViewer<FieldViewer>()->release(m_cellAsset);
			getParentViewer()->getParentViewer<MainViewer>()->addCellAssetViewer(m_cellAsset);
		}
	}
}
