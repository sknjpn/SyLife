#include "MainViewer.h"
#include "GUIButton.h"
#include "GUIText.h"

void MainViewer::CellMakingViewer::PartsAssembler::init()
{
	setViewerRectInLocal(300, 0, 1200, 800);

	addChildViewer<Workspace>()
		->setCellAsset(getParentViewer<CellMakingViewer>()->getCellAsset());

	addChildViewer<PartList>();

	addChildViewer<GUIButton>([this]() { getChildViewer<Workspace>()->setMoveMode(); }, false)
		->setName(U"移動モード")
		->setViewerRectInLocal(5, 5, 190, 30)
		->addChildViewer<GUIText>(U"移動モード", Font(24, Typeface::Bold))
		->mouseoverDisable();

	addChildViewer<GUIButton>([this]() { getChildViewer<Workspace>()->setRotateMode(); })
		->setName(U"回転モード")
		->setViewerRectInLocal(5, 45, 190, 30)
		->addChildViewer<GUIText>(U"回転モード", Font(24, Typeface::Bold))
		->mouseoverDisable();
}

void MainViewer::CellMakingViewer::PartsAssembler::update()
{

}
