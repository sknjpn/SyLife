#include "CellMakingViewer.h"
#include "CellAsset.h"
#include "PartAsset_Body.h"
#include "GUIChecker.h"
#include "GUIButton.h"
#include "GUIText.h"
#include "GUIValuer.h"

void CellMakingViewer::BodySculptor::init()
{
	m_stamp = Circle(10).asPolygon();

	setViewerRectInLocal(200, 0, 1200, 1100);

	const auto ba = getParentViewer<CellMakingViewer>()->getCellAsset()->getBodyAsset();
	const auto& ll = addChildViewer<LayerLists>();
	const auto& w = addChildViewer<Workspace>();

	ll->setPartAsset(ba);
	w->setPartAsset(ba);

	// 左右対象の設定
	{
		m_isSymmetrical = false;

		addChildViewer<GUIChecker>()
			->setName(U"左右対称")
			->setViewerRectInLocal(5, 5, 30, 30);

		addChildViewer<GUIText>(U"左右対称")
			->setViewerRectInLocal(35, 5, 165, 30);


		addChildViewer<GUIText>(U"スタンプ種類")
			->setViewerRectInLocal(5, 50, 165, 30);

		addChildViewer<GUIButton>(U"丸いスタンプ", [this]() {m_stamp = Circle(10).asPolygon(); })
			->setViewerRectInLocal(5, 80, 190, 30);

		addChildViewer<GUIButton>(U"四角いスタンプ", [this]() {m_stamp = RectF(10.0, 10.0).setCenter(Vec2::Zero()).asPolygon(); })
			->setViewerRectInLocal(5, 120, 190, 30);


		addChildViewer<GUIText>(U"スタンプサイズ")
			->setViewerRectInLocal(5, 170, 165, 30);

		addChildViewer<GUIValuer>(0.1)
			->setName(U"スタンプサイズ")
			->setViewerRectInLocal(5, 200, 190, 30);
	}
}

void CellMakingViewer::BodySculptor::update()
{
}

Polygon CellMakingViewer::BodySculptor::getStamp() const
{
	double value = getChildViewer<GUIValuer>(U"スタンプサイズ")->getValue();

	return m_stamp.scaled(Math::Lerp(0.5, 5.0, value));
}
