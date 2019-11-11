#include "CellStateViewer.h"

#include "CellAsset.h"
#include "CellState.h"
#include "ElementAsset.h"

CellStateViewer::CellStateViewer()
	: m_cellState(nullptr)
{
	setViewerRect(Scene::Width() - 300, 450, 200, 400);
}

void CellStateViewer::update()
{
	Rect(getViewerRect().size.asPoint()).draw(Color(11, 22, 33, 192));

	if (!m_cellState) return;
	if (m_cellState->isDestroyed())
	{
		m_cellState = nullptr;

		return;
	}

	const auto& asset = m_cellState->m_cellAsset;

	{
		auto t1 = Transformer2D(Mat3x2::Translate(100, 50));
		auto t2 = Transformer2D(Mat3x2::Scale(50.0 / m_cellState->m_cellAsset->getRadius()));

		m_cellState->m_cellAsset->draw(1.0);
	}

	Vec2 pos(0, 100);
	static Font font(13, Typeface::Bold);

	{
		{
			auto t = Transformer2D(Mat3x2::Translate(pos));
			font(U"Material").draw();
			pos.moveBy(0, 16);
		}

		pos.moveBy(16.0, 0.0);
		for (const auto& m : asset->getMaterial().getElementList())
		{
			auto t = Transformer2D(Mat3x2::Translate(pos));
			font(m.first->getName() + U":" + ToString(m.second) + U"U").draw();
			pos.moveBy(0, 15);
		}
		pos.moveBy(0, 32);
	}

	{
		pos.moveBy(-16.0, 0.0);
		{
			auto t = Transformer2D(Mat3x2::Translate(pos));
			font(U"Storage").draw();
			pos.moveBy(0, 16);
		}

		pos.moveBy(16.0, 0.0);
		for (const auto& m : m_cellState->m_storage.getElementList())
		{
			auto t = Transformer2D(Mat3x2::Translate(pos));
			font(m.first->getName() + U":" + ToString(m.second) + U"U").draw();
			pos.moveBy(0, 15);
		}
		pos.moveBy(-16.0, 0.0);
	}
}
