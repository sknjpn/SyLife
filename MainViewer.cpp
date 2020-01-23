#include "MainViewer.h"
#include "GUIButton.h"

void MainViewer::openCellMakingViewer()
{
	if (!hasChildViewer<CellMakingViewer>()) addChildViewer<CellMakingViewer>();
}

void MainViewer::init()
{
	addChildViewer<FieldViewer>();

	addChildViewer<GUIButton>(U"生き物作成", [this]() { openCellMakingViewer(); })->setViewerRectInLocal(100, 50, 250, 50);

	addChildViewer<CellBook>();

	addChildViewer<StatisticsViewer>();

	addChildViewer<CommandPalette>();
}

void MainViewer::update()
{

}
