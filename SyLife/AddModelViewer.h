#pragma once

#include "Viewer.h"

#include "AssetManager.h"

class AddModelViewer
	: public Viewer
{
public:
	template <typename T>
	void	addModel()
	{
		for (int i = 1;; ++i)
		{
			String name = U"model_" + ToString(i);

			if (!g_assetManagerPtr->hasAsset(name))
			{
				auto m = g_assetManagerPtr->makeAsset<T>();

				m->setName(name);

				return;
			}
		}
	}

	template <typename T>
	void	drawAssets(const String& name)
	{
		static Font font10(10, Typeface::Bold);
		auto f = font10(name);

		f.region().draw(ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
		f.draw();

		if (f.region().leftClicked()) addModel<T>();

		moveDrawPos(0, 15);
	}

	void	update() override;
	void	init() override;
};

