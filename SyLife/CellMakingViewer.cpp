#include "CellMakingViewer.h"

#include "ViewerManager.h"

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

	}
	else
	{
		static Font font(32, Typeface::Bold);

		font(U"Create Cell").drawAt(getDrawSize() / 2.0);
	}
}

void CellMakingViewer::open()
{
	m_isOpened = true;

	// Viewerの初期化
	g_viewerManagerPtr->makeViewer<AssemblyViewer>();
	g_viewerManagerPtr->makeViewer<ReleaseViewer>();
	g_viewerManagerPtr->makeViewer<PartPaletteViewer>();
}

void CellMakingViewer::close()
{
	m_isOpened = false;

	setDrawRect(RectF(200, 50).setCenter(Scene::CenterF().x, 50));
}
