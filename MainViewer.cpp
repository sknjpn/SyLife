#include "MainViewer.h"
#include "GUIButton.h"

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

	addChildViewer<CommandPalette>();
}

void MainViewer::update()
{

}