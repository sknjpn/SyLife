#include "CellMakingViewer.h"
#include "GUIButton.h"

void CellMakingViewer::PartsAssembler::init()
{
	setViewerRectInLocal(200, 0, 1200, 800);

	addChildViewer<Workspace>()
		->setCellAsset(getParentViewer<CellMakingViewer>()->getCellAsset());

	addChildViewer<PartList>();

	addChildViewer<GUIButton>(U"移動モード", [this]() { getChildViewer<Workspace>()->setMoveMode(); }, false)
		->setName(U"移動モード")
		->setViewerRectInLocal(5, 80, 190, 30);

	addChildViewer<GUIButton>(U"回転モード", [this]() { getChildViewer<Workspace>()->setRotateMode(); })
		->setName(U"回転モード")
		->setViewerRectInLocal(5, 120, 190, 30);

}

void CellMakingViewer::PartsAssembler::update()
{

}
