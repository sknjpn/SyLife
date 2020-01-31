#include "MainViewer.h"
#include "GUIMusicBox.h"
#include "GUIButton.h"
#include "CellState.h"
#include "GUICurtain.h"

void MainViewer::openCellMakingViewer()
{
	if (!hasChildViewer<CellMakingViewer>()) addChildViewer<CellMakingViewer>();
}

void MainViewer::setHiddenMode()
{
	m_hiddenMode = true;

	for (auto hv : getChildViewers<HiddenViewer>())
		hv->moveToSecondPos();

	if (auto viewer = getChildViewer<GUIButton>()) { viewer->destroy(); }
	if (auto viewer = getChildViewer<CellMakingViewer>()) { viewer->destroy(); }

	for (auto viewer : getChildViewers<CellAssetViewer>())
		viewer->destroy();

	getChildViewer<FieldViewer>()->m_handAction = FieldViewer::HandAction::None;
}

void MainViewer::unsetHiddenMode()
{
	m_hiddenMode = false;

	for (auto hv : getChildViewers<HiddenViewer>())
		hv->moveToFirstPos();

	addChildViewer<GUIButton>(U"生き物作成", [this]() { openCellMakingViewer(); })->setViewerRectInLocal(100, 50, 250, 50);
}

void MainViewer::init()
{
	addChildViewer<FieldViewer>();

	addChildViewer<GUIButton>(U"生き物作成", [this]() { openCellMakingViewer(); })->setViewerRectInLocal(100, 50, 250, 50);

	addChildViewer<CellBook>();

	addChildViewer<CommandPalette>();

	if (GeneralSetting::GetInstance().m_audioEnabled)
		addChildViewer<GUIMusicBox>(U"", false);

	// OpenCurtain
	addChildViewer<GUICurtain>(Color(11, 22, 33), Color(0, 0), 0.5);

	if (GeneralSetting::GetInstance().m_autoTurnOutEnabled)
		m_uncontrolTimer.start();
}

void MainViewer::update()
{
	// HiddenModeの実行
	if (GeneralSetting::GetInstance().m_autoTurnOutEnabled)
	{
		if (m_uncontrolTimer.isRunning() && MouseL.pressed()) m_uncontrolTimer.reset();
		if (MouseL.up()) m_uncontrolTimer.start();

		if (m_uncontrolTimer.s() > GeneralSetting::GetInstance().m_autoTurnOutTime)
		{
			if (!m_hiddenMode) setHiddenMode();
		}
		else
		{
			if (m_hiddenMode) unsetHiddenMode();
		}
	}

	if (auto musicBox = getChildViewer<GUIMusicBox>())
	{
		const Array<String> musicList = { U"かみさまのゆりかご", U"沈む。", U"真相探求" };

		if (!musicBox->isPlaying()) musicBox->setMusic(musicList.choice());
	}
}

void MainViewer::addCellAssetViewer(const std::shared_ptr<CellAsset>& cellAsset)
{
	if (!hasChildViewer(cellAsset->getName()))
	{
		addChildViewer<CellAssetViewer>(cellAsset)
			->setViewerPosInLocal(Vec2(1.0, 1.0).setLength(50.0 * getChildViewers<CellAssetViewer>().size()));
	}
}

void MainViewer::addCellAssetViewer(const std::shared_ptr<CellState>& cellState)
{
	if (hasChildViewer(cellState->getCellAsset()->getName()))
	{
		getChildViewer<CellAssetViewer>(cellState->getCellAsset()->getName())
			->setCellState(cellState);

		getChildViewer<CellAssetViewer>(cellState->getCellAsset()->getName())
			->moveToFront();
	}
	else
	{
		addChildViewer<CellAssetViewer>(cellState)
			->setViewerPosInLocal(Vec2(1.0, 1.0).setLength(50.0 * getChildViewers<CellAssetViewer>().size()));
	}
}
