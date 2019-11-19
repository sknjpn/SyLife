﻿#include "CellMakingViewer.h"
#include "Shape.h"
#include "PartAsset.h"
#include "GUISlider.h"

void CellMakingViewer::BodySculptor::LayerLists::init()
{
	setViewerRectInLocal(1000, 0, 200, 1000);
	addChildViewer<GUISlider>(0)->setViewerRectInLocal(RectF(200 - 24, 0, 24, 800));
}

void CellMakingViewer::BodySculptor::LayerLists::update()
{
	if (!m_partAsset) return;

	getChildViewer<GUISlider>()->setHeight((m_partAsset->m_shape.size() + 1) * m_itemHeight);

	moveDrawPos(0, -getChildViewer<GUISlider>()->getDelta());
	{
		drawLayers();

		// 新規Layer
		{
			static Font font(18, Typeface::Bold);

			const auto block = RectF(170, m_itemHeight).stretched(-2.0);
			block.draw(ColorF(1.0, block.mouseOver() ? 0.5 : 0.25)).drawFrame(1.0, Palette::White);

			font(U"Layerの追加").drawAt(block.center(), Palette::Black);

			if (block.leftClicked())
			{
				// 新しいLayerの追加
				auto& l = m_partAsset->m_shape.emplace_back();
				l.m_color = Palette::White;
				l.m_polygon = Circle(10.0).asPolygon();

				// 選択中のLayerを変更
				m_selectedIndex = int(m_partAsset->m_shape.size()) - 1;
			}

			moveDrawPos(0, m_itemHeight);
		}
	}
	moveDrawPos(0, getChildViewer<GUISlider>()->getDelta());
}

void CellMakingViewer::BodySculptor::LayerLists::setPartAsset(const shared_ptr<PartAsset>& partAsset)
{
	m_partAsset = partAsset;
	m_selectedIndex = 0;
}

void CellMakingViewer::BodySculptor::LayerLists::drawLayers()
{
	for (auto it = m_partAsset->m_shape.begin(); it != m_partAsset->m_shape.end(); ++it)
	{
		const auto block = RectF(170, m_itemHeight).stretched(-2.0);
		block.draw((distance(m_partAsset->m_shape.begin(), it) == m_selectedIndex) ? ColorF(Palette::Blue, 0.5) : ColorF(1.0, block.mouseOver() ? 0.5 : 0.25)).drawFrame(1.0, Palette::White);
		if (block.leftClicked()) m_selectedIndex = int(it - m_partAsset->m_shape.begin());

		// ColorPicker
		it->m_color = m_colorPicker.update(it->m_color);

		// パーツ描画
		{
			RectF rect = it->m_polygon.boundingRect();
			auto scale = Min((m_itemHeight - 20) / rect.w, (m_itemHeight - 20) / rect.h);
			auto t = Transformer2D(Mat3x2::Translate(-rect.center()).scaled(scale).translated(Vec2(170 - m_itemHeight / 2.0, m_itemHeight / 2.0)));

			it->m_polygon.draw(it->m_color);
		}

		moveDrawPos(0, m_itemHeight);
	}
}