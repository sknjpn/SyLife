#pragma once

#include "ModelViewer.h"
#include "ShapeModel.h"

class PartModel;

class PartPaletteViewer
	: public ModelViewer
{
public:
	shared_ptr<PartModel>	m_selectedPart;

public:
	PartPaletteViewer()
	{
		SetDrawRect(Scene::Width() - 100, 20, 80, 400);
	}

	template <typename T>
	void	DrawModels(const string& name)
	{
		static Font font13(12, Typeface::Bold);
		static Font font10(10, Typeface::Bold);

		font13(Unicode::Widen(name)).draw();
		MoveDrawPos(0, 16);

		const auto& models = g_assetManagerPtr->GetModels<T>();
		for (auto it = models.begin(); it != models.end(); ++it)
		{
			Rect(64, 64).stretched(-2).draw(ColorF(1.0, 0.25)).drawFrame(1.0, Palette::White);

			MoveDrawPos(4, 4);
			{
				auto f = font10(Unicode::Widen((*it)->GetName()));
				f.region().draw(ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
				f.draw();

				if (f.region().leftClicked()) m_selectedPart = *it;

				// ƒp[ƒc•`‰æ
				for (const ShapeModel& s : (*it)->GetShapes())
				{
					s.Draw(0.5);
				}
			}
			MoveDrawPos(-4, -4);

			MoveDrawPos(0, 64);
		}
	}

	void	Update() override;
};

