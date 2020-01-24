#include "MainViewer.h"
#include "Shape.h"
#include "PartAsset.h"
#include "PartAsset_Body.h"
#include "GUISlider.h"

void MainViewer::CellMakingViewer::BodySculptor::LayerLists::init()
{
	setViewerRectInLocal(1000, 0, 200, 1000);
	addChildViewer<GUISlider>(0)->setViewerRectInLocal(RectF(200 - 24, 0, 24, 800));

	for (int i = 0; i < m_bodyAsset->getShape().size(); ++i)
	{
		const auto& item = addChildViewer<Item>(m_bodyAsset);
		item->setViewerSize(170, m_itemHeight);
		item->setHSV(m_bodyAsset->getShape()[i].m_color);
	}
	getChildViewers<Item>().back()->setSelected(true);

	m_selectedIndex = 0;
}

void MainViewer::CellMakingViewer::BodySculptor::LayerLists::update()
{
	if (!m_bodyAsset) return;

	getChildViewer<GUISlider>()->setHeight((m_bodyAsset->getShape().size() + 1) * m_itemHeight);

	const auto delta = -getChildViewer<GUISlider>()->getDelta();

	// Layer
	for (int i = 0; i < getChildViewers<Item>().size(); ++i)
	{
		const auto& item = getChildViewers<Item>()[i];

		item->setViewerPosInLocal(0, delta + m_itemHeight * i);
		m_bodyAsset->getShape()[i].m_color = item->getHSV();

		if (item->isMouseover() && MouseL.down())
		{
			for (const auto& item2 : getChildViewers<Item>()) item2->setSelected(false);
			item->setSelected(true);

			m_selectedIndex = i;
		}
	}

	// 新規Layer
	moveDrawPos(0, delta + m_itemHeight * getChildViewers<Item>().size());
	{
		static Font font(18, Typeface::Bold);

		const auto block = RectF(170, m_itemHeight).stretched(-2.0);
		block.draw(ColorF(1.0, block.mouseOver() ? 0.5 : 0.25)).drawFrame(1.0, Palette::White);

		font(U"レイヤーの追加").drawAt(block.center(), Palette::Black);

		if (block.leftClicked())
		{
			// 新しいLayerの追加
			auto& layer = m_bodyAsset->getShape().emplace_back();
			layer.m_color = Palette::White;
			layer.m_polygon = Circle(10.0).asPolygon();

			for (const auto& item : getChildViewers<Item>()) item->setSelected(false);

			const auto& item = addChildViewer<Item>(m_bodyAsset);
			item->setSelected(true);
			item->setViewerSize(170, m_itemHeight);

			m_selectedIndex = int(getChildViewers<Item>().size() - 1);
		}
	}
}
