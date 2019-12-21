#include "CellMakingViewer.h"

void CellMakingViewer::PartsAssembler::init()
{
	setViewerRectInLocal(200, 0, 1200, 800);

	addChildViewer<Workspace>()
		->setCellAsset(getParentViewer<CellMakingViewer>()->getCellAsset());

	addChildViewer<PartList>();
}

void CellMakingViewer::PartsAssembler::update()
{

}
