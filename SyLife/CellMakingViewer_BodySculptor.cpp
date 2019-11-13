#include "CellMakingViewer.h"
#include "CellAsset.h"
#include "BodyAsset.h"

void CellMakingViewer::BodySculptor::init()
{
	setViewerRectInLocal(200, 0, 1200, 800);

	const auto ba = getParentViewer<CellMakingViewer>()->getCellAsset()->getBodyAsset();
	const auto& ll = addChildViewer<LayerLists>();
	const auto& w = addChildViewer<Workspace>();

	ll->setPartAsset(ba);
	w->setPartAsset(ba);
}

void CellMakingViewer::BodySculptor::update()
{

}
