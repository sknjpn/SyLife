#include "CellAssetViewer.h"
#include "CellAsset.h"
#include "PartConfig.h"
#include "PartAsset.h"

CellAssetViewer::CellAssetViewer(const shared_ptr<CellAsset>& cellAsset)
	: m_cellAsset(cellAsset)
{
	setViewerSize(200, 800);
}

void CellAssetViewer::update()
{
	// Font
	static Font font(20);
	const double d = 25;

	// 背景
	{
		RectF rect(getViewerSize());
		rect.stretched(-10).draw(Palette::White).drawFrame(5.0, 0.0, Palette::Black);
	}

	if (!m_cellAsset->isValid())
	{
		font(U"有効でないCellAsset").draw();
		return;
	}

	font(U"CellAsset").draw();
	moveDrawPos(0, d);
	font(U"Name=" + Unicode::Widen(m_cellAsset->getName())).draw();
	moveDrawPos(0, d);
	font(U"Mass=", m_cellAsset->getMass()).draw();
	moveDrawPos(0, d);
	font(U"Radius=", m_cellAsset->getRadius()).draw();
	moveDrawPos(0, d);
	font(U"Inertia=", m_cellAsset->getInertia()).draw();
	moveDrawPos(0, d);
	font(U"NumParts=", m_cellAsset->getPartConfigs().size()).draw();
	moveDrawPos(0, d);
	font(U"Parts").draw();
	moveDrawPos(0, d);
	for (auto it = m_cellAsset->getPartConfigs().begin(); it != m_cellAsset->getPartConfigs().end(); ++it)
	{
		font(U"PartName=" + Unicode::Widen((*it)->getPartAsset()->getName())).draw();
		moveDrawPos(0, d);
	}
}
