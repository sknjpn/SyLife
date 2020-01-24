#include "MainViewer.h"
#include "GUIMusicBox.h"
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

	INIData ini(U"config.ini");
	if (ini.getOr<bool>(U"General", U"BGM", true))
		addChildViewer<GUIMusicBox>(U"resources/music/かみさまのゆりかご.mp3");
}

void MainViewer::update()
{

}

void MainViewer::addCellAssetViewer(const shared_ptr<CellAsset>& cellAsset)
{
	if (!hasChildViewer(cellAsset->getName()))
	{
		addChildViewer<CellAssetViewer>(cellAsset)
			->setViewerPosInLocal(Vec2(1.0, 1.0).setLength(50.0 * getChildViewers<CellAssetViewer>().size()));
	}
}
