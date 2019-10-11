#include "ShapeLayerViewer.h"

#include "ShapeModel.h"
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
}

void ShapeLayerViewer::drawLayers()
{
	static Font font(13, Typeface::Bold);

	auto shapes = m_partAsset->getShapes();
	for (auto it = shapes.begin(); it != shapes.end(); ++it)
	{
		const auto block = RectF(170, m_itemHeight).stretched(-2.0);
		block.draw((int(it - shapes.begin()) == m_selectedIndex) ? ColorF(Palette::Blue, 0.5) : ColorF(1.0, block.mouseOver() ? 0.5 : 0.25)).drawFrame(1.0, Palette::White);
		if (block.leftClicked()) m_selectedIndex = int(it - shapes.begin());

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
