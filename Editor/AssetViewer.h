#pragma once

#include "Viewer.h"
#include "Model.h"

class AssetViewer
	: public Viewer
{
	shared_ptr<Model>	m_selectedModel;
	s3d::Vec2	m_drawPos;

public:
	AssetViewer()
	{
		m_drawRect = s3d::RectF(600, 0, 200, 600);
	}

	void	Update() override;
	void	SetSelectedModel(const shared_ptr<Model>& model);

	template <typename T>
	void	DrawModels(const string& text)
	{
		static s3d::Font font13(13);
		static s3d::Font font10(10, s3d::Typeface::Bold);

		// Type
		{
			auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(m_drawPos), true);

			font13(s3d::Unicode::Widen(text)).draw();
			m_drawPos.moveBy(0.0, 16.0);
		}

		// Models
		{
			m_drawPos.moveBy(16.0, 0.0);
			const auto& models = g_assetManagerPtr->GetModels<T>();
			for (auto it = models.begin(); it != models.end(); ++it)
			{
				auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(m_drawPos), true);
				auto f = font10(s3d::Unicode::Widen((*it)->m_name));

				f.region().draw(m_selectedModel == *it ? s3d::Palette::Darkorange : s3d::ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
				f.draw();

				if (f.region().leftClicked()) SetSelectedModel(*it);

				m_drawPos.moveBy(0.0, 15.0);
			}
			m_drawPos.moveBy(-16.0, 0.0);
		}
	}
};

