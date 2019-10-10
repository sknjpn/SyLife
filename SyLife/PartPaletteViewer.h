#pragma once

#include "Viewer.h"

#include "AssetManager.h"

#include "ShapeModel.h"

class PartAsset;

class PartPaletteViewer
	: public Viewer
{
	double	m_barWidth = 20;
	double	m_itemWidth = 80;
	double	m_bar = 0.0;
	shared_ptr<PartAsset>	m_selectedPart;

public:
	PartPaletteViewer()
	{
		SetDrawRect(Scene::Width() - m_barWidth - m_itemWidth, 20, m_barWidth + m_itemWidth, 400);
	}

	const shared_ptr<PartAsset>& getSelectedPart() const { return m_selectedPart; }

	void	clearSelectedPart() { m_selectedPart = nullptr; }

	template <typename T>
	void	drawModels(const string& name)
	{
		static Font font13(15, Typeface::Bold);
		static Font font10(13, Typeface::Bold);

		font13(Unicode::Widen(name)).draw();
		MoveDrawPos(0, 20);

		const auto& models = g_assetManagerPtr->getAssets<T>();
		for (auto it = models.begin(); it != models.end(); ++it)
		{
			const auto block = RectF(m_itemWidth, m_itemWidth).stretched(-2.0);
			block.draw(ColorF(1.0, block.mouseOver() ? 0.5 : 0.25)).drawFrame(1.0, Palette::White);
			if (block.leftClicked()) m_selectedPart = *it;

			// 名前描画
			font10(Unicode::Widen((*it)->getName())).draw(4, 4);

			// パーツ描画
			{
				RectF rect = (*it)->getApproximateRect();
				auto scale = Min((m_itemWidth - 8) / rect.w, (m_itemWidth - 8) / rect.h);
				auto t = Transformer2D(Mat3x2::Scale(scale).translated(-rect.center() + Vec2(m_itemWidth / 2.0, m_itemWidth / 2.0)));

				for (const ShapeModel& s : (*it)->getShapes())
					s.draw(0.5);
			}

			MoveDrawPos(0, m_itemWidth);
		}

		MoveDrawPos(0, 16);
	}

	void	update() override;
};

