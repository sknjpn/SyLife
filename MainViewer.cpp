#include "MainViewer.h"

#include "CellStateViewer.h"
#include "GUIButton.h"
#include "StatisticsViewer.h"
#include "CurtainViewer.h"
#include "FieldViewer.h"
#include "CellMakingViewer.h"

void MainViewer::openCellMakingViewer()
{
	if (!hasChildViewer<CellMakingViewer>()) addChildViewer<CellMakingViewer>();
}

void MainViewer::init()
{
	addChildViewer<FieldViewer>();

	addChildViewer<CellStateViewer>();

	addChildViewer<GUIButton>(U"Cell作成", [this]() { openCellMakingViewer(); })->setViewerRectInLocal(100, 50, 200, 50);

	addChildViewer<StatisticsViewer>();

	// OpenCurtain
	addChildViewer<CurtainViewer>(Color(11, 22, 33), Color(0, 0), 0.5);
}

void MainViewer::update()
{

}