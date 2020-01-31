#include "MainViewer.h"
#include "CellAsset.h"
#include "PartAsset_Body.h"
#include "GUIChecker.h"
#include "GUIButton.h"
#include "GUIText.h"
#include "GUIValuer.h"

void MainViewer::CellMakingViewer::BodySculptor::setState(State state)
{
	m_state = state;

	switch (state)
	{
	case CellMakingViewer::BodySculptor::State::Put:
		getChildViewer<GUIButton>(U"付けるスタンプ")->setIsEnabled(false);
		getChildViewer<GUIButton>(U"削るスタンプ")->setIsEnabled(true);
		break;
	case CellMakingViewer::BodySculptor::State::Shave:
		getChildViewer<GUIButton>(U"付けるスタンプ")->setIsEnabled(true);
		getChildViewer<GUIButton>(U"削るスタンプ")->setIsEnabled(false);
		break;
	default:
		break;
	}
}

void MainViewer::CellMakingViewer::BodySculptor::init()
{
	setViewerRectInLocal(300, 0, 1200, 1100);

	const auto bodyAsset = getParentViewer<CellMakingViewer>()->getCellAsset()->getBodyAsset();
	addChildViewer<LayerLists>(bodyAsset);
	addChildViewer<Workspace>(bodyAsset);

	// 左右対象の設定
	{
		m_isSymmetrical = false;

		addChildViewer<GUIChecker>()
			->setName(U"左右対称")
			->setViewerRectInLocal(5, 5, 30, 30);

		addChildViewer<GUIText>(U"左右対称", Font(18, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
			->setViewerRectInLocal(40, 5, 165, 30);


		addChildViewer<GUIText>(U"スタンプ種類", Font(18, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
			->setViewerRectInLocal(5, 50, 165, 30);

		addChildViewer<GUIButton>([this]() { setState(State::Put); })
			->setName(U"付けるスタンプ")
			->setViewerRectInLocal(5, 80, 190, 30)
			->addChildViewer<GUIText>(U"付けるスタンプ", Font(24, Typeface::Bold));

		addChildViewer<GUIButton>([this]() { setState(State::Shave); })
			->setName(U"削るスタンプ")
			->setViewerRectInLocal(5, 120, 190, 30)
			->addChildViewer<GUIText>(U"削るスタンプ", Font(24, Typeface::Bold));

		addChildViewer<GUIText>(U"スタンプサイズ", Font(18, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
			->setViewerRectInLocal(5, 170, 165, 30);

		addChildViewer<GUIValuer>(0.1)
			->setName(U"スタンプサイズ")
			->setViewerRectInLocal(5, 200, 190, 30);
	}

	setState(State::Put);
}

void MainViewer::CellMakingViewer::BodySculptor::update()
{
}

double MainViewer::CellMakingViewer::BodySculptor::getStampRadius() const
{
	return getChildViewer<GUIValuer>(U"スタンプサイズ")->getValue() * 100.0;
}
