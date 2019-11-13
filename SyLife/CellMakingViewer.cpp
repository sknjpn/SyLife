#include "CellMakingViewer.h"
#include "AssetManager.h"
#include "PartConfig.h"
#include "PartAsset.h"
#include "BodyAsset.h"
#include "ElementAsset.h"
#include "CellAsset.h"
#include "GUIButton.h"
#include "ReleaseViewer.h"

void CellMakingViewer::clearEditor()
{
	if (const auto v = getChildViewer<BodySculptor>()) v->destroy();
	if (const auto v = getChildViewer<PartsAssembler>()) v->destroy();
}

void CellMakingViewer::openBodySculptor()
{
	clearEditor();

	addChildViewer<BodySculptor>();
}

void CellMakingViewer::openPartsAssembler()
{
	clearEditor();

	addChildViewer<PartsAssembler>();
}

void CellMakingViewer::init()
{
	setBackgroundColor(Color(11, 22, 33));

	// 新しいモデルの登録
	makeAsset();

	// DrawRectの設定
	setViewerRectInLocal(RectF(1400, 900).setCenter(Scene::CenterF()));

	addChildViewer<GUIButton>(U"ボディ編集", [this]() { openBodySculptor(); })
		->setViewerRectInLocal(5, 5, 190, 35);

	addChildViewer<GUIButton>(U"パーツ配置", [this]() { openPartsAssembler(); })
		->setViewerRectInLocal(5, 45, 190, 35);

	addChildViewer<GUIButton>(U"生き物配置", [this]() { getParentViewer()->addChildViewer<ReleaseViewer>(m_cellAsset); destroy(); })
		->setViewerRectInLocal(5, 85, 190, 35);

	addChildViewer<GUIButton>(U"閉じる", [this]() { destroy(); })
		->setViewerRectInLocal(5, 125, 190, 35);

	addChildViewer<CellInfo>()
		->setViewerRectInLocal(0, 165, 200, 595);
}

void CellMakingViewer::update()
{
	// 更新
	m_cellAsset->updateProperties();
	for (const auto& pc : m_cellAsset->getPartConfigs())
		pc->getPartAsset()->m_shape.updateProperties();
}

void CellMakingViewer::makeAsset()
{
	m_cellAsset = g_assetManagerPtr->makeAsset<CellAsset>();

	// 名前をランダムに設定
	{
		TextReader textReader(U"assets/names.txt");

		m_cellAsset->setName(textReader.readAll().split_lines().choice());
	}

	// Bodyの設定
	{
		auto bodyAsset = g_assetManagerPtr->makeAsset<BodyAsset>();
		m_cellAsset->addPartConfig()->setPartAsset(bodyAsset);

		bodyAsset->m_mass = 1.0;
		bodyAsset->m_material.setNutrition(1.0);
		auto& l = bodyAsset->m_shape.emplace_back();
		l.m_color = Palette::White;
		l.m_polygon = Circle(10.0).asPolygon();
	}

	m_cellAsset->updateProperties();
}
