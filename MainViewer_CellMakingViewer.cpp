﻿#include "MainViewer.h"
#include "PartConfig.h"
#include "PartAsset.h"
#include "PartAsset_Body.h"
#include "PartAsset_Nucleus.h"
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

	getChildViewer<GUIButton>(U"EditBody")->setIsEnabled(false);
	getChildViewer<GUIButton>(U"EditPart")->setIsEnabled(true);

	addChildViewer<BodySculptor>();
}

void MainViewer::CellMakingViewer::openPartsAssembler()
{
	clearEditor();

	getChildViewer<GUIButton>(U"EditBody")->setIsEnabled(true);
	getChildViewer<GUIButton>(U"EditPart")->setIsEnabled(false);

	addChildViewer<PartsAssembler>();
}

void MainViewer::CellMakingViewer::release()
{
	getParentViewer()->getChildViewer<FieldViewer>()->release(m_cellAsset);
	getParentViewer()->getChildViewer<CellBook>()->addItem(m_cellAsset);

	destroy();
}

void MainViewer::CellMakingViewer::init()
{
	// 新しいモデルの登録
	makeAsset();

	// DrawRectの設定
	setViewerRectInLocal(RectF(1500, 800).setCenter(Scene::CenterF()));

	addChildViewer<GUIButton>(U"ボディ編集", [this]() { openBodySculptor(); })
		->setName(U"EditBody")
		->setViewerRectInLocal(5, 5, 290, 35);

	addChildViewer<GUIButton>(U"パーツ配置", [this]() { openPartsAssembler(); })
		->setName(U"EditPart")
		->setViewerRectInLocal(5, 45, 290, 35);

	addChildViewer<GUIButton>(U"生き物配置", [this]() { release(); }, false)
		->setName(U"生き物配置")
		->setViewerRectInLocal(5, 85, 290, 35);

	addChildViewer<GUIButton>(U"閉じる", [this]() { destroy(); })
		->setViewerRectInLocal(5, 125, 290, 35);

	addChildViewer<CellInfo>()
		->setViewerRectInLocal(5, 165, 290, 595);

	openBodySculptor();
}

void MainViewer::CellMakingViewer::update()
{
	RectF(getViewerSize()).rounded(16.0).draw(Palette::Gray).drawFrame(2, 0, Palette::Black);

	// 更新
	m_cellAsset->updateProperties();
	for (const auto& partConfig : m_cellAsset->getPartConfigs())
		partConfig->getPartAsset()->getShape().updateProperties();

	getChildViewer<GUIButton>(U"生き物配置")->setIsEnabled(m_cellAsset->isValid());
}

void MainViewer::CellMakingViewer::makeAsset()
{
	m_cellAsset = World::MakeAsset<CellAsset>();

	Logger << m_cellAsset->getName();

	// 名前をランダムに設定
	{
		TextReader textReader(U"resources/names.txt");

		m_cellAsset->setNameJP(textReader.readAll().split_lines().choice());
	}

	// Bodyの設定
	{
		auto bodyAsset = World::MakeAsset<PartAsset_Body>();
		m_cellAsset->addPartConfig()->setPartAsset(bodyAsset);

		bodyAsset->setMass(1.0);
		bodyAsset->getMaterial().setElement(1.0);
		auto& l = bodyAsset->getShape().emplace_back();
		l.m_color = Palette::White;
		l.m_polygon = Circle(15.0).asPolygon().simplified(0.5);
	}

	// Nucleusの設定
	{
		auto nucleusAsset = World::GetAssets<PartAsset_Nucleus>().back();

		m_cellAsset->addPartConfig()->setPartAsset(nucleusAsset);
	}

	m_cellAsset->updateProperties();
}
