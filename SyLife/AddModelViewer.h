#pragma once

#include "Viewer.h"
class AddModelViewer
	: public Viewer
{
	Vec2	m_drawPos;
public:
	AddModelViewer()
	{
		SetDrawRect(100, 150, 400, 300);
	}

	template <typename T>
	void	AddModel()
	{
		for (int i = 1;; ++i)
		{
			string name = "model_" + to_string(i);

			if (!g_assetManagerPtr->HasModel(name))
			{
				auto m  = g_assetManagerPtr->MakeModel<T>();

				m->SetName(name);

				return;
			}
		}
	}

	template <typename T>
	void	DrawModels(const string& name)
	{
		static Font font10(10, Typeface::Bold);
		auto t = Transformer2D(Mat3x2::Translate(m_drawPos), true);
		auto f = font10(Unicode::Widen(name));

		f.region().draw(ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
		f.draw();

		if (f.region().leftClicked()) AddModel<T>();

		m_drawPos.moveBy(0.0, 15.0);
	}

	void	Update() override;
};

