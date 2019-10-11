#include "PartPaletteViewer.h"

#include "PartAsset.h"

#include "BodyAsset.h"
#include "EquipmentAsset.h"
#include "ModuleAsset.h"

PartPaletteViewer::PartPaletteViewer()
	: m_slideBar(800, 800 / 8.0)
{
	setPriority(2);
}

void PartPaletteViewer::drawModels()
{
	static Font font(13, Typeface::Bold);

	const auto& models = g_assetManagerPtr->getAssets<PartAsset>();
	for (auto it = models.begin(); it != models.end(); ++it)
	{
		const auto block = RectF(170, m_itemHeight).stretched(-2.0);
		block.draw(ColorF(1.0, block.mouseOver() ? 0.5 : 0.25)).drawFrame(1.0, Palette::White);
		if (block.leftClicked()) m_selectedPart = *it;

		// パーツ描画
		{
			RectF rect = (*it)->getApproximateRect();
			auto scale = Min((m_itemHeight - 20) / rect.w, (m_itemHeight - 20) / rect.h);
			auto t = Transformer2D(Mat3x2::Scale(scale).translated(-rect.center() + Vec2(170 - m_itemHeight/2.0, m_itemHeight / 2.0)));

			for (const ShapeModel& s : (*it)->getShapes())
				s.draw(0.5);
		}

		// 名前描画
		font(Unicode::Widen((*it)->getName())).draw(4, 4);

		moveDrawPos(0, m_itemHeight);
	}
}

void	PartPaletteViewer::update()
{
	// Bar
	{
		auto t = Transformer2D(Mat3x2::Translate(200 - 30, 0), true);

		m_slideBar.update();
	}

	{
		const double h = g_assetManagerPtr->getAssets<PartAsset>().size() * m_itemHeight - 800;

		moveDrawPos(0, -h * m_slideBar.getValue());

		drawModels();
	}
}