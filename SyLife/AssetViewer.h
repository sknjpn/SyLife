#pragma once

#include "Viewer.h"
#include "Model.h"

class AssetViewer
	: public Viewer
{
	shared_ptr<Model>	m_selectedModel;
	Vec2	m_drawPos;

public:
	AssetViewer()
	{
		SetDrawRect(600, 0, 200, 600);
	}

	void	Update() override;
	void	SetSelectedModel(const shared_ptr<Model>& model);

	template <typename T>
	void	DrawModels(const string& text)
	{
		static Font font13(13);
		static Font font10(10, Typeface::Bold);

		// Type
		{
			auto t = Transformer2D(Mat3x2::Translate(m_drawPos), true);

			font13(Unicode::Widen(text)).draw();
			m_drawPos.moveBy(0.0, 16.0);
		}

		// Models
		{
			m_drawPos.moveBy(16.0, 0.0);
			const auto& models = g_assetManagerPtr->GetModels<T>();
			for (auto it = models.begin(); it != models.end(); ++it)
			{
				auto t = Transformer2D(Mat3x2::Translate(m_drawPos), true);
				auto f = font10(Unicode::Widen((*it)->GetName()));

				f.region().draw(m_selectedModel == *it ? Palette::Darkorange : ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
				f.draw();

				if (f.region().leftClicked()) SetSelectedModel(*it);

				m_drawPos.moveBy(0.0, 15.0);
			}
			m_drawPos.moveBy(-16.0, 0.0);
		}
	}
};

