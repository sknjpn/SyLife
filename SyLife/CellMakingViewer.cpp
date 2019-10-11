#include "CellMakingViewer.h"

#include "ViewerManager.h"

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

		av->setDrawRect(RectF(800, 800).setCenter(getDrawCenter().movedBy(0, -50)).stretched(-5));
		ppv->setDrawRect(RectF(200, 900).setCenter(getDrawCenter().movedBy(500, 0)).stretched(-5));
		// rv->setDrawRect(RectF()
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
	g_viewerManagerPtr->makeViewer<AssemblyViewer>();
	g_viewerManagerPtr->makeViewer<ReleaseViewer>();
	g_viewerManagerPtr->makeViewer<PartPaletteViewer>();
}

void CellMakingViewer::close()
{
	m_isOpened = false;

	// DrawRectの設定
	setDrawRect(RectF(200, 50).setCenter(Scene::CenterF().x, 50));
}
