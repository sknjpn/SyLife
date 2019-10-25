#include "ShapeLayerViewer.h"

#include "Shape.h"
#include "PartAsset.h"

ShapeLayerViewer::ShapeLayerViewer()
	: m_slideBar(800, 800 / 8.0)
{
	setPriority(2);

	setDrawRect(RectF(200, 800).setCenter(getDrawCenter().movedBy(500, -50)));
}

void ShapeLayerViewer::init()
{
}

void ShapeLayerViewer::update()
{
	if (!m_partAsset || isInvisible()) return;

	drawLayers();

	// 新規Layer
	{
		static Font font(13, Typeface::Bold);

		const auto block = RectF(170, m_itemHeight).stretched(-2.0);
		block.draw(ColorF(1.0, block.mouseOver() ? 0.5 : 0.25)).drawFrame(1.0, Palette::White);

		font(U"Add Shape").drawAt(block.center());

		if (block.leftClicked())
		{
			// 新しいShapeの追加
			auto& s = m_partAsset->m_shapes.emplace_back();
			s.m_color = Palette::White;
			s.m_polygon = Circle(10.0).asPolygon();

			// 選択中のShapeを変更
			m_selectedIndex = int(m_partAsset->m_shapes.size()) - 1;
		}

		moveDrawPos(0, m_itemHeight);
	}
}

void ShapeLayerViewer::setPartAsset(const shared_ptr<PartAsset>& partAsset)
{
	m_partAsset = partAsset;
	m_selectedIndex = 0;
}

void ShapeLayerViewer::drawLayers()
{
	auto& shapes = m_partAsset->m_shapes;
	for (auto it = shapes.begin(); it != shapes.end(); ++it)
	{
		const auto block = RectF(170, m_itemHeight).stretched(-2.0);
		block.draw((int(it - shapes.begin()) == m_selectedIndex) ? ColorF(Palette::Blue, 0.5) : ColorF(1.0, block.mouseOver() ? 0.5 : 0.25)).drawFrame(1.0, Palette::White);
		if (block.leftClicked()) m_selectedIndex = int(it - shapes.begin());

		// ColorPicker
		it->m_color = m_colorPicker.update(it->m_color);

		// パーツ描画
		{
			RectF rect = it->m_polygon.boundingRect();
			auto scale = Min((m_itemHeight - 20) / rect.w, (m_itemHeight - 20) / rect.h);
			auto t = Transformer2D(Mat3x2::Scale(scale).translated(-rect.center() + Vec2(170 - m_itemHeight / 2.0, m_itemHeight / 2.0)));

			it->m_polygon.draw(it->m_color);
		}

		moveDrawPos(0, m_itemHeight);
	}
}
