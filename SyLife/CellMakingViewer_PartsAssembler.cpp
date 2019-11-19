#include "CellMakingViewer.h"

void CellMakingViewer::PartsAssembler::init()
{
	setViewerRectInLocal(200, 0, 1200, 800);

	const auto ca = getParentViewer<CellMakingViewer>()->getCellAsset();
	const auto& w = addChildViewer<Workspace>();
	const auto& pl = addChildViewer<PartList>();

	w->setCellAsset(ca);
}

void CellMakingViewer::PartsAssembler::update()
{

}
