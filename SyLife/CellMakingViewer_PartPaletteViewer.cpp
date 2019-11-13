#include "CellMakingViewer.h"
#include "AssetManager.h"
#include "PartAsset.h"
#include "BodyAsset.h"
#include "EquipmentAsset.h"
#include "NucleusAsset.h"
#include "ModuleAsset.h"
#include "GUISlider.h"

Array<shared_ptr<PartAsset>> CellMakingViewer::PartPaletteViewer::getList() const
{
	Array<shared_ptr<PartAsset>> assets;

	for (const auto& m : g_assetManagerPtr->getAssets<EquipmentAsset>()) assets.emplace_back(m);
	for (const auto& m : g_assetManagerPtr->getAssets<EquipmentAsset>()) assets.emplace_back(m);
	for (const auto& m : g_assetManagerPtr->getAssets<EquipmentAsset>()) assets.emplace_back(m);
	for (const auto& m : g_assetManagerPtr->getAssets<EquipmentAsset>()) assets.emplace_back(m);
	for (const auto& m : g_assetManagerPtr->getAssets<EquipmentAsset>()) assets.emplace_back(m);
	for (const auto& m : g_assetManagerPtr->getAssets<EquipmentAsset>()) assets.emplace_back(m);
	for (const auto& m : g_assetManagerPtr->getAssets<ModuleAsset>()) assets.emplace_back(m);
	for (const auto& m : g_assetManagerPtr->getAssets<NucleusAsset>()) assets.emplace_back(m);
	for (const auto& m : g_assetManagerPtr->getAssets<NucleusAsset>()) assets.emplace_back(m);
	for (const auto& m : g_assetManagerPtr->getAssets<NucleusAsset>()) assets.emplace_back(m);
	for (const auto& m : g_assetManagerPtr->getAssets<NucleusAsset>()) assets.emplace_back(m);
	for (const auto& m : g_assetManagerPtr->getAssets<NucleusAsset>()) assets.emplace_back(m);
	for (const auto& m : g_assetManagerPtr->getAssets<NucleusAsset>()) assets.emplace_back(m);
	for (const auto& m : g_assetManagerPtr->getAssets<NucleusAsset>()) assets.emplace_back(m);
	
	return assets;
}

void CellMakingViewer::PartPaletteViewer::drawAssets()
{
	static Font font(13, Typeface::Bold);

	const auto assets = getList();
	for (auto it = assets.begin(); it != assets.end(); ++it)
	{
		const auto block = RectF(170, m_itemHeight).stretched(-2.0);
		block.draw(ColorF(1.0, block.mouseOver() ? 0.5 : 0.25)).drawFrame(1.0, Palette::White);
		if (block.leftClicked()) m_selectedPart = *it;

		// パーツ描画
		{
			RectF rect = (*it)->getShape().getRect();
			auto scale = Min((m_itemHeight - 20) / rect.w, (m_itemHeight - 20) / rect.h);
			auto t = Transformer2D(Mat3x2::Translate(-rect.center()).scaled(scale).translated(Vec2(170 - m_itemHeight / 2.0, m_itemHeight / 2.0)));

			(*it)->draw(0.5);
		}

		// 名前描画
		font((*it)->getName()).draw(4, 4);

		moveDrawPos(0, m_itemHeight);
	}
}

void CellMakingViewer::PartPaletteViewer::init()
{
	setViewerRect(RectF(200, 800).setCenter(getDrawCenter().movedBy(500, -50)));
	addChildViewer<GUISlider>(getList().size() * m_itemHeight)->setViewerRect(RectF(200 - 24, 0, 24, 800).movedBy(getViewerRect().pos));
}

void	CellMakingViewer::PartPaletteViewer::update()
{
	getChildViewer<GUISlider>()->setHeight(getList().size() * m_itemHeight);

	moveDrawPos(0, -getChildViewer<GUISlider>()->getDelta());
	drawAssets();
	moveDrawPos(0, getChildViewer<GUISlider>()->getDelta());
}
