#include "MainViewer.h"
#include "PartConfig.h"
#include "PartAsset.h"
#include "PartAsset_Body.h"
#include "ProteinAsset.h"
#include "CellAsset.h"
#include "GUIButton.h"
#include "World.h"

void MainViewer::CellMakingViewer::clearEditor()
{
	if (const auto v = getChildViewer<BodySculptor>()) v->destroy();
	if (const auto v = getChildViewer<PartsAssembler>()) v->destroy();
}

void MainViewer::CellMakingViewer::openBodySculptor()
{
	clearEditor();

	addChildViewer<BodySculptor>();
}

void MainViewer::CellMakingViewer::openPartsAssembler()
{
	clearEditor();

	addChildViewer<PartsAssembler>();
}

void MainViewer::CellMakingViewer::init()
{
	// 新しいモデルの登録
	makeAsset();

	// DrawRectの設定
	setViewerRectInLocal(RectF(1400, 800).setCenter(Scene::CenterF()));

	addChildViewer<GUIButton>(U"ボディ編集", [this]() { openBodySculptor(); })
		->setViewerRectInLocal(5, 5, 190, 35);

	addChildViewer<GUIButton>(U"パーツ配置", [this]() { openPartsAssembler(); })
		->setViewerRectInLocal(5, 45, 190, 35);

	addChildViewer<GUIButton>(U"生き物配置", [this]() { getParentViewer()->getChildViewer<FieldViewer>()->release(m_cellAsset); destroy(); })
		->setName(U"生き物配置")
		->setViewerRectInLocal(5, 85, 190, 35);

	addChildViewer<GUIButton>(U"閉じる", [this]() { destroy(); })
		->setViewerRectInLocal(5, 125, 190, 35);

	addChildViewer<CellInfo>()
		->setViewerRectInLocal(0, 165, 200, 595);

	openBodySculptor();

	getChildViewer<GUIButton>(U"生き物配置")->setIsEnabled(false);
}

void MainViewer::CellMakingViewer::update()
{
	RectF(getViewerSize()).rounded(16.0).draw(Palette::Gray).drawFrame(2,0, Palette::Black);

	// 更新
	m_cellAsset->updateProperties();
	for (const auto& pc : m_cellAsset->getPartConfigs())
		pc->getPartAsset()->getShape().updateProperties();

	getChildViewer<GUIButton>(U"生き物配置")->setIsEnabled(m_cellAsset->isValid());
}

void MainViewer::CellMakingViewer::makeAsset()
{
	m_cellAsset = World::MakeAsset<CellAsset>();

	Logger << m_cellAsset->getName();

	// 名前をランダムに設定
	{
		TextReader textReader(U"resources/names.txt");

		m_cellAsset->setName(textReader.readAll().split_lines().choice());
	}

	// Bodyの設定
	{
		auto bodyAsset = World::MakeAsset<PartAsset_Body>();
		m_cellAsset->addPartConfig()->setPartAsset(bodyAsset);

		bodyAsset->setMass(1.0);
		bodyAsset->getMaterial().setElement(1.0);
		auto& l = bodyAsset->getShape().emplace_back();
		l.m_color = Palette::White;
		l.m_polygon = Circle(10.0).asPolygon();
	}

	m_cellAsset->updateProperties();
}
