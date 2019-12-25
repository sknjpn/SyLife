#include "MainViewer.h"

void MainViewer::CommandPalette::init()
{
	setViewerRectInLocal(1700, 50, 200, 100);

	addChildViewer<MagnifyingViewer>()
		->setViewerPosInLocal(0, 0);

	addChildViewer<SpeedControllerViewer>()
		->setViewerPosInLocal(0, 40);
}

void MainViewer::CommandPalette::update()
{
	RectF(getViewerSize()).rounded(5).draw(Palette::White).drawFrame(2.0, 0.0, Palette::Black);
}
