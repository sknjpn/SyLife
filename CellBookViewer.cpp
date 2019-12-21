#include "CellBookViewer.h"
#include "CellAsset.h"

void CellBookViewer::init()
{
	setViewerRectInLocal(RectF(1000, 1000).setCenter(Scene::Center()));

	setBackgroundColor(Palette::Gray);
	addChildViewer<CellList>();
}

void CellBookViewer::update()
{

}
