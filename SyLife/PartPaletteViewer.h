#pragma once

#include "Viewer.h"

#include "AssetManager.h"

class PartAsset;

class PartPaletteViewer
	: public Viewer
{
	double	m_bar = 0.0;
	shared_ptr<PartAsset>	m_selectedPart;

public:
	PartPaletteViewer()
	{
		SetDrawRect(Scene::Width() - 100, 20, 80, 400);
	}

	const shared_ptr<PartAsset>& getSelectedPart() const { return m_selectedPart; }

	void	clearSelectedPart() { m_selectedPart = nullptr; }

	template <typename T>
	void	drawModels(const string& name)
	{
		static Font font13(12, Typeface::Bold);
		static Font font10(10, Typeface::Bold);

		font13(Unicode::Widen(name)).draw();
		MoveDrawPos(0, 16);

		const auto& models = g_assetManagerPtr->getAssets<T>();
		for (auto it = models.begin(); it != models.end(); ++it)
		{
			const auto block = Rect(64, 64).stretched(-2);
			block.draw(ColorF(1.0, block.mouseOver() ? 0.5 : 0.25)).drawFrame(1.0, Palette::White);
			if (block.leftClicked()) m_selectedPart = *it;

			MoveDrawPos(4, 4);
			{
				font10(Unicode::Widen((*it)->getName())).draw();


				// パーツ描画
				{
					RectF rect = (*it)->getApproximateRect();
					auto scale = Min(56 / rect.w, 56 / rect.h);
					auto t = Transformer2D(Mat3x2::Scale(scale).translated(-rect.center() + Vec2(28, 28)));

					for (const ShapeModel& s : (*it)->getShapes())
						s.draw(0.5);
				}
			}
			MoveDrawPos(-4, -4);

			MoveDrawPos(0, 64);
		}

		MoveDrawPos(0, 16);
	}

	void	update() override;
};

