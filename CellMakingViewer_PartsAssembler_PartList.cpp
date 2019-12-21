#include "CellMakingViewer.h"
#include "World.h"
#include "PartAsset.h"
#include "PartAsset_Body.h"
#include "PartAsset_Nucleus.h"
#include "GUISlider.h"

void CellMakingViewer::PartsAssembler::PartList::init()
{
	setViewerRectInLocal(1000, 0, 200, 800);
	addChildViewer<GUISlider>(getList().size() * m_itemHeight)->setViewerRectInLocal(RectF(200 - 24, 0, 24, 800));
}

void	CellMakingViewer::PartsAssembler::PartList::update()
{
	getChildViewer<GUISlider>()->setHeight(getList().size() * m_itemHeight);

	moveDrawPos(0, -getChildViewer<GUISlider>()->getDelta());
	drawAssets();
	moveDrawPos(0, getChildViewer<GUISlider>()->getDelta());
}

Array<shared_ptr<PartAsset>> CellMakingViewer::PartsAssembler::PartList::getList() const
{
	Array<shared_ptr<PartAsset>> assets;

	for (const auto& m : Assets::GetAssets<PartAsset>().removed_if([](const auto& pa) { return dynamic_pointer_cast<PartAsset_Body>(pa) ? true : false; })) assets.emplace_back(m);
	
	return assets;
}

void CellMakingViewer::PartsAssembler::PartList::drawAssets()
{
	static Font font(16, Typeface::Bold);

	const auto assets = getList();
	for (auto it = assets.begin(); it != assets.end(); ++it)
	{
		const auto block = RectF(170, m_itemHeight).stretched(-2.0);
		block.draw(ColorF(1.0, block.mouseOver() ? 0.5 : 0.25)).drawFrame(1.0, Palette::White);
		if (block.leftClicked()) m_selectedPart = *it;

		// パーツ描画
		{
			RectF rect = (*it)->getShape().getChipSize();
			auto scale = Min((m_itemHeight - 20) / rect.w, (m_itemHeight - 20) / rect.h);
			auto t = Transformer2D(Mat3x2::Translate(-rect.center()).scaled(scale).translated(Vec2(170 - m_itemHeight / 2.0, m_itemHeight / 2.0)));

			(*it)->draw(0.5);
		}

		// 名前描画
		font((*it)->getName()).draw(4, 4, Palette::Black);

		moveDrawPos(0, m_itemHeight);
	}
}
