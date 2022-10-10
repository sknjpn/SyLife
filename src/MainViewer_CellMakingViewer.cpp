#include "CellAsset.h"
#include "GUIButton.h"
#include "GUIText.h"
#include "MainViewer.h"
#include "PartAsset.h"
#include "PartConfig.h"
#include "Part_BodyAsset.h"
#include "Part_NucleusAsset.h"
#include "ProteinAsset.h"
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
  m_cellAsset->setCentroidAsOrigin();
  m_cellAsset->updateProperties();

  // Render
  m_cellAsset->preRender();

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

  addChildViewer<GUIButton>([this]() { openBodySculptor(); })
    ->setName(U"EditBody")
    ->setViewerRectInLocal(5, 5, 290, 35)
    ->addChildViewer<GUIText>(U"ボディ編集", Font(28, Typeface::Bold));

  addChildViewer<GUIButton>([this]() { openPartsAssembler(); })
    ->setName(U"EditPart")
    ->setViewerRectInLocal(5, 45, 290, 35)
    ->addChildViewer<GUIText>(U"パーツ配置", Font(28, Typeface::Bold));

  addChildViewer<GUIButton>([this]() { release(); }, false)
    ->setName(U"生き物配置")
    ->setViewerRectInLocal(5, 85, 290, 35)
    ->addChildViewer<GUIText>(U"生き物配置", Font(28, Typeface::Bold));

  addChildViewer<GUIButton>([this]() { destroy(); })
    ->setViewerRectInLocal(5, 125, 290, 35)
    ->addChildViewer<GUIText>(U"閉じる", Font(28, Typeface::Bold));

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

  // 名前をランダムに設定
  {
    TextReader textReader(U"resources/names.txt");

    m_cellAsset->setNameJP(textReader.readAll().split_lines().choice());
  }

  // Bodyの設定
  {
    auto bodyAsset = World::MakeAsset<Part_BodyAsset>();
    m_cellAsset->addPartConfig()->setPartAsset(bodyAsset);

    bodyAsset->setMass(1.0);
    bodyAsset->getMaterial().setElement(1.0);
    auto& l = bodyAsset->getShape().emplace_back();
    l.m_color = Palette::White;
    l.m_polygon = Circle(15.0).asPolygon();
    Circle(15.0).asPolygon().scaled(4.0).movedBy(400, 400).overwrite(bodyAsset->m_image, Palette::White, Antialiased::Yes);
  }

  // Nucleusの設定
  {
    auto nucleusAsset = World::GetAssets<Part_NucleusAsset>().back();

    m_cellAsset->addPartConfig()->setPartAsset(nucleusAsset);
  }

  m_cellAsset->updateProperties();
}
