#include "MainViewer.h"

void MainViewer::CommandPalette::init()
{
	setViewerSize(2, 2);
	addChildViewer<MagnifyingViewer>()
		->setViewerPosInLocal(Scene::Size().x / 2.0 - 40, 20);
}

void MainViewer::CommandPalette::update()
{
}
