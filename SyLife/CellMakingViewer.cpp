#include "CellMakingViewer.h"

#include "ViewerManager.h"

#include "AssemblyViewer.h"
#include "ReleaseViewer.h"
#include "PartPaletteViewer.h"

void CellMakingViewer::init()
{
	m_isOpened = false;

	// Viewerの初期化
	g_viewerManagerPtr->makeViewer<AssemblyViewer>();
	g_viewerManagerPtr->makeViewer<ReleaseViewer>();
	g_viewerManagerPtr->makeViewer<PartPaletteViewer>();
}

void CellMakingViewer::update()
{
	if (m_isOpened)
	{

	}
	else
	{

	}
}
