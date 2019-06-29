#pragma once

#include "Viewer.h"
#include "AssetManager.h"

class AddModelViewer
	: public Viewer
{
	s3d::Vec2	m_drawPos;
public:
	AddModelViewer()
	{
		m_drawRect = s3d::RectF(100, 150, 400, 300);
	}

	template <typename T>
	void	AddModel()
	{
		for (int i = 1;; ++i)
		{
			string name = "model_" + to_string(i);

			if (!g_assetManagerPtr->HasModel(name))
			{
				auto m  = g_assetManagerPtr->AddModel<T>(name);

				return;
			}
		}
	}

	template <typename T>
	void	DrawModels(const string& name)
	{
		static s3d::Font font10(10, s3d::Typeface::Bold);
		auto t = s3d::Transformer2D(s3d::Mat3x2::Translate(m_drawPos), true);
		auto f = font10(s3d::Unicode::Widen(name));

		f.region().draw(s3d::ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
		f.draw();

		if (f.region().leftClicked()) AddModel<T>();

		m_drawPos.moveBy(0.0, 15.0);
	}

	void	Update() override;
};

