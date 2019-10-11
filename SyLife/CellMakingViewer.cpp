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
	setPriority(1);

	// サブViewer生成
	g_viewerManagerPtr->makeViewer<AssemblyViewer>()->setInvisible(true);
	g_viewerManagerPtr->makeViewer<PartPaletteViewer>()->setInvisible(true);

	// 新しいモデルの登録
	m_cellAsset = g_assetManagerPtr->makeAsset<CellAsset>();

	close();
}

void CellMakingViewer::update()
{
	switch (m_mode)
	{
	case CellMakingViewer::Mode::Close:
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
	break;
	case CellMakingViewer::Mode::EditParts:
		// 閉じる
		if (KeyEscape.down())
		{
			close();

			return;
		}

		auto av = g_viewerManagerPtr->getViewer<AssemblyViewer>();
		auto ppv = g_viewerManagerPtr->getViewer<PartPaletteViewer>();

		// Release
		{
			const RectF rect = Rect(200, 200).stretched(-5);
			const double r = rect.size.x / 2.0;
			setDrawPos(rect.pos);

			Circle(rect.size / 2.0, r)
				.draw(Palette::Skyblue)
				.drawFrame(4.0, Palette::Black);

			// part
			{
				auto t1 = Transformer2D(Mat3x2::Scale(r / m_cellAsset->getRadius() / 2.0).translated(rect.center()));

				for (const auto& p : m_cellAsset->getPartConfigs())
				{
					auto t2 = Transformer2D(Mat3x2::Rotate(p->getRotation())
						.translated(p->getPosition().x, p->getPosition().y));

					for (const auto& s : p->getModel()->getShapes())
						s.m_polygon.draw(ColorF(s.m_color, 0.5)).drawFrame(1.0, Palette::Black);
				}
			}
		}

		// Release Button
		{
			setDrawPos(5, 210);

			if (SimpleGUI::Button(U"Release", Vec2(0, 0), 180))
			{
				auto rv = g_viewerManagerPtr->makeViewer<ReleaseViewer>();

				m_isReleasing = true;

				setInvisible(true);
				av->setInvisible(true);
				ppv->setInvisible(true);
			}
		}

		// Close Button
		{
			setDrawPos(5, 250);

			if (SimpleGUI::Button(U"Close", Vec2(0, 0), 180))
			{
				close();

				return;
			}
		}

		// material
		{
			setDrawPos(Vec2(0, 290));

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
		break;
	case CellMakingViewer::Mode::EditBodyShapes:
		break;
	case CellMakingViewer::Mode::Release:
		break;
	default:
		break;
	}
}

void CellMakingViewer::open()
{
	m_mode = Mode::EditBodyShapes;

	// BackgroundColorの設定
	setBackgroundColor(Color(11, 22, 33));

	// DrawRectの設定
	setDrawRect(RectF(1200, 900).setCenter(Scene::CenterF()));

	// Viewerの初期化
	auto av = g_viewerManagerPtr->getViewer<AssemblyViewer>();
	auto ppv = g_viewerManagerPtr->getViewer<PartPaletteViewer>();
	av->setInvisible(false);
	ppv->setInvisible(false);

	av->setDrawRect(RectF(800, 800).setCenter(getDrawCenter().movedBy(0, -50)));
	av->setSize(Vec2(800, 800));
	ppv->setDrawRect(RectF(200, 800).setCenter(getDrawCenter().movedBy(500, -50)));
}

void CellMakingViewer::close()
{
	m_mode = Mode::Close;

	// サブViewerの非表示
	auto av = g_viewerManagerPtr->getViewer<AssemblyViewer>();
	auto ppv = g_viewerManagerPtr->getViewer<PartPaletteViewer>();
	av->setInvisible(true);
	ppv->setInvisible(true);

	// 本体の表示
	setInvisible(false);

	// DrawRectの設定
	setDrawRect(RectF(200, 50).setCenter(Scene::CenterF().x, 50));
}

void CellMakingViewer::release()
{
	m_mode = Mode::Release;

	// 新規Asset生成
	m_cellAsset = g_assetManagerPtr->makeAsset<CellAsset>();

	close();
}
