#include "CellAssetViewer.h"
#include "CellAsset.h"
#include "PartConfig.h"
#include "PartAsset.h"

CellAssetViewer::CellAssetViewer(const shared_ptr<CellAsset>& cellAsset)
	: m_cellAsset(cellAsset)
{
	setViewerSize(250, 800);
}

void CellAssetViewer::update()
{
	// Font
	static Font font(16, Typeface::Bold);
	const double d = 20;

	// 背景
	{
		RectF rect(getViewerSize());
		rect.rounded(10).draw(Palette::White).drawFrame(5.0, 0.0, Palette::Black);
	}

	moveDrawPos(10, 10);

	if (!m_cellAsset->isValid())
	{
		font(U"有効でないCellAsset").draw(Vec2::Zero(), Palette::Black);
		return;
	}

	font(U"CellAsset").draw(Vec2::Zero(), Palette::Black);
	moveDrawPos(0, d);
	font(U"Name=" + m_cellAsset->getName()).draw(Vec2::Zero(), Palette::Black);
	moveDrawPos(0, d);
	font(U"Mass=", m_cellAsset->getMass()).draw(Vec2::Zero(), Palette::Black);
	moveDrawPos(0, d);
	font(U"Radius=", m_cellAsset->getRadius()).draw(Vec2::Zero(), Palette::Black);
	moveDrawPos(0, d);
	font(U"Inertia=", m_cellAsset->getInertia()).draw(Vec2::Zero(), Palette::Black);
	moveDrawPos(0, d);
	font(U"NumParts=", m_cellAsset->getPartConfigs().size()).draw(Vec2::Zero(), Palette::Black);
	moveDrawPos(0, d);
	font(U"Parts").draw(Vec2::Zero(), Palette::Black);
	moveDrawPos(0, d);
	for (auto it = m_cellAsset->getPartConfigs().begin(); it != m_cellAsset->getPartConfigs().end(); ++it)
	{
		font(U"PartName=" + (*it)->getPartAsset()->getName()).draw(Vec2::Zero(), Palette::Black);
		moveDrawPos(0, d);
	}
}
