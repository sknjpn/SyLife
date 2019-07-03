#pragma once

#include "ModelViewer.h"

class PartModel;

class PartPaletteViewer
	: public ModelViewer
{
public:
	shared_ptr<PartModel>	m_selectedPart;

	Vec2	m_drawPos;

public:
	PartPaletteViewer()
	{
		SetDrawRect(Window::Width() - 100, 20, 80, 400);
	}

	template <typename T>
	void	DrawModels(const string& name)
	{
		static Font font13(12, Typeface::Bold);
		static Font font10(10, Typeface::Bold);

		{
			auto t = Transformer2D(Mat3x2::Translate(m_drawPos), true);

			font13(Unicode::Widen(name)).draw();
			m_drawPos.moveBy(0.0, 16.0);
		}

		m_drawPos.moveBy(16.0, 0.0);
		const auto& models = g_assetManagerPtr->GetModels<T>();
		for (auto it = models.begin(); it != models.end(); ++it)
		{
			auto t = Transformer2D(Mat3x2::Translate(m_drawPos), true);
			auto f = font10(Unicode::Widen((*it)->GetName()));

			f.region().draw(ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
			f.draw();

			if (f.region().leftClicked()) m_selectedPart = *it;

			m_drawPos.moveBy(0.0, 15.0);
		}
		m_drawPos.moveBy(-16.0, 0.0);
	}

	void	Update() override;
};

