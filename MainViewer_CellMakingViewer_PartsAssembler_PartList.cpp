﻿#include "MainViewer.h"
#include "PartConfig.h"
#include "PartAsset.h"
#include "PartAsset_Body.h"
#include "PartAsset_Nucleus.h"
#include "GUISlider.h"
#include "CellAsset.h"
#include "World.h"

void MainViewer::CellMakingViewer::PartsAssembler::PartList::init()
{
	setViewerRectInLocal(1000, 0, 200, 800);
	addChildViewer<GUISlider>(getList().size() * m_itemHeight)->setViewerRectInLocal(RectF(200 - 24, 0, 24, 800));
}

void	MainViewer::CellMakingViewer::PartsAssembler::PartList::update()
{
	getChildViewer<GUISlider>()->setHeight(getList().size() * m_itemHeight);

	moveDrawPos(0, -getChildViewer<GUISlider>()->getDelta());
	drawAssets();
	moveDrawPos(0, getChildViewer<GUISlider>()->getDelta());
}

Array<std::shared_ptr<PartAsset>> MainViewer::CellMakingViewer::PartsAssembler::PartList::getList() const
{
	Array<std::shared_ptr<PartAsset>> assets;

	for (const auto& m : World::GetAssets<PartAsset>().removed_if([](const auto& pa) { return dynamic_pointer_cast<PartAsset_Body>(pa) ? true : false; })) assets.emplace_back(m);

	return assets;
}

void MainViewer::CellMakingViewer::PartsAssembler::PartList::drawAssets()
{
	static Font font(16, Typeface::Bold);

	const auto assets = getList();
	for (auto it = assets.begin(); it != assets.end(); ++it)
	{
		const auto block = RectF(170, m_itemHeight).stretched(-2.0);
		const bool isNucleus = dynamic_pointer_cast<PartAsset_Nucleus>(*it) != nullptr;

		if (isNucleus)
		{
			block.draw(ColorF(Palette::Yellow, block.mouseOver() ? 0.5 : 0.25)).drawFrame(1.0, Palette::White);

			if (block.leftClicked())
			{
				auto cellAsset = getParentViewer()->getParentViewer<CellMakingViewer>()->getCellAsset();

				for (const auto& pc : cellAsset->getPartConfigs())
				{
					if (dynamic_pointer_cast<PartAsset_Nucleus>(pc->getPartAsset()) != nullptr)
					{
						cellAsset->removePartConfig(pc);

						break;
					}
				}

				cellAsset->addPartConfig()->setPartAsset(*it);
			}
		}
		else
		{
			block.draw(ColorF(1.0, block.mouseOver() ? 0.5 : 0.25)).drawFrame(1.0, Palette::White);

			if (block.leftClicked()) m_selectedPart = *it;
		}

		// パーツ描画
		{
			RectF rect = (*it)->getShape().getTileSize();
			auto scale = Min((m_itemHeight - 20) / rect.w, (m_itemHeight - 20) / rect.h);
			auto t = Transformer2D(Mat3x2::Translate(-rect.center()).scaled(scale).translated(Vec2(170 - m_itemHeight / 2.0, m_itemHeight / 2.0)));

			(*it)->draw(0.5);
		}

		// 名前描画
		font((*it)->getNameJP()).draw(4, 4, Palette::Black);

		moveDrawPos(0, m_itemHeight);
	}
}
