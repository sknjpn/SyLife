#include "CellMakingViewer.h"

#include "ViewerManager.h"

#include "PartConfig.h"
#include "PartAsset.h"
#include "ElementAsset.h"
#include "CellAsset.h"
#include "AssemblyViewer.h"
#include "ReleaseViewer.h"
#include "PartPaletteViewer.h"

void CellMakingViewer::init()
{
	close();
}

void CellMakingViewer::update()
{
	if (m_isOpened)
	{
		auto av = g_viewerManagerPtr->getViewer<AssemblyViewer>();
		auto rv = g_viewerManagerPtr->getViewer<ReleaseViewer>();
		auto ppv = g_viewerManagerPtr->getViewer<PartPaletteViewer>();

		// Release
		{
			const RectF rect = Rect(200, 200).stretched(-5);
			const double r = rect.size.x / 2.0;
			setDrawPos(rect.pos);

			Circle circle(rect.size / 2.0, r);

			circle
				.draw(circle.mouseOver() ? Palette::Orange : Palette::Skyblue)
				.drawFrame(4.0, Palette::Black);

			/*if (m_isDragged) circle.draw(Palette::Red);
			if (circle.leftClicked()) m_isDragged = true;
			if (!MouseL.pressed()) m_isDragged = false;
			*/

			// part
			{
				auto t1 = Transformer2D(Mat3x2::Scale(r / m_cellAsset->getRadius() / 2.0).translated(circle.center));

				for (const auto& p : m_cellAsset->getPartConfigs())
				{
					auto t2 = Transformer2D(Mat3x2::Rotate(p->getRotation())
						.translated(p->getPosition().x, p->getPosition().y));

					for (const auto& s : p->getModel()->getShapes())
						s.m_polygon.draw(ColorF(s.m_color, 0.5)).drawFrame(1.0, Palette::Black);
				}
			}
		}

		// material
		{
			setDrawPos(Vec2(0, 200));

			static Font font(13, Typeface::Bold);

			// Nutrition
			font(U"Nutrition: " + ToString(m_cellAsset->getMaterial().getNutrition())).draw();
			moveDrawPos(0, 20);

			// Elements
			for (const auto& e : m_cellAsset->getMaterial().getElementList())
			{
				font(Unicode::Widen(e.first->getName()) + U": " + ToString(e.second) + U"U").draw();

				moveDrawPos(0, 16);
			}
		}
	}
	else
	{
		// Push
		{
			static bool isClicked = false;


			if (isMouseOver())
			{
				if (MouseL.down()) isClicked = true;

				if (MouseL.up())
				{
					isClicked = false;

					open();

					return;
				}

				if (isClicked && MouseL.pressed()) setBackgroundColor(ColorF(1.0, 0.3));
				else setBackgroundColor(ColorF(1.0, 0.5));
			}
			else
			{
				if (MouseL.up()) isClicked = false;

				setBackgroundColor(ColorF(1.0, 0.1));
			}
		}

		// Text
		{
			static Font font(32, Typeface::Bold);

			font(U"Create Cell").drawAt(getDrawSize() / 2.0);
		}
	}
}

void CellMakingViewer::open()
{
	m_isOpened = true;

	// BackgroundColorの設定
	setBackgroundColor(Palette::Deepskyblue);

	// DrawRectの設定
	setDrawRect(RectF(1200, 900).setCenter(Scene::CenterF()));

	// 新しいモデルの登録
	m_cellAsset = g_assetManagerPtr->makeAsset<CellAsset>();

	// Viewerの初期化
	auto av = g_viewerManagerPtr->makeViewer<AssemblyViewer>();
	auto rv = g_viewerManagerPtr->makeViewer<ReleaseViewer>();
	auto ppv = g_viewerManagerPtr->makeViewer<PartPaletteViewer>();

	av->setDrawRect(RectF(800, 800).setCenter(getDrawCenter().movedBy(0, -50)).stretched(-5));
	av->setSize(Vec2(800, 800) - Vec2(10, 10));
	ppv->setDrawRect(RectF(200, 900).setCenter(getDrawCenter().movedBy(500, 0)).stretched(-5));
}

void CellMakingViewer::close()
{
	m_isOpened = false;

	// DrawRectの設定
	setDrawRect(RectF(200, 50).setCenter(Scene::CenterF().x, 50));
}
