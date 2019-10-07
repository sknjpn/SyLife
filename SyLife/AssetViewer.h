#pragma once

#include "Viewer.h"
#include "Model.h"

#include "AssetManager.h"

class AssetViewer
	: public Viewer
{
	shared_ptr<Model>	m_selectedModel;

public:
	AssetViewer()
	{
		SetDrawRect(0, 0, 400, 1080);
	}

	void	update() override;
	void	SetSelectedModel(const shared_ptr<Model>& model);

	template <typename T>
	void	DrawModels(const string& text)
	{
		static Font font13(13);
		static Font font10(10, Typeface::Bold);

		// Type
		font13(Unicode::Widen(text)).draw();
		MoveDrawPos(0, 16);

		// Models
		{
			MoveDrawPos(16, 0);
			const auto& models = g_assetManagerPtr->GetModels<T>();
			for (auto it = models.begin(); it != models.end(); ++it)
			{
				auto f = font10(Unicode::Widen((*it)->getName()));

				f.region().draw(m_selectedModel == *it ? ColorF(Palette::Darkorange) : ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
				f.draw();

				if (f.region().leftClicked()) SetSelectedModel(*it);

				MoveDrawPos(0, 15);
			}
			MoveDrawPos(-16, 0);
		}
	}
};

