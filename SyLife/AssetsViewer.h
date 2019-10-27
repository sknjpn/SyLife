#pragma once

#include "Viewer.h"
#include "Asset.h"

#include "AssetManager.h"

class AssetsViewer
	: public Viewer
{
	shared_ptr<Asset>	m_selectedAsset;

public:
	AssetsViewer()
	{
		setViewerRect(0, 0, 400, 1080);
	}

	void	update() override;

	template <typename T>
	void	drawModels(const string& text)
	{
		static Font font13(13);
		static Font font10(10, Typeface::Bold);

		// Type
		font13(Unicode::Widen(text)).draw();
		moveDrawPos(0, 16);

		// Models
		{
			moveDrawPos(16, 0);
			const auto& assets = g_assetManagerPtr->getAssets<T>();
			for (auto it = assets.begin(); it != assets.end(); ++it)
			{
				auto f = font10(Unicode::Widen((*it)->getName()));

				f.region().draw(m_selectedAsset == *it ? ColorF(Palette::Darkorange) : ColorF(1.0, f.region().mouseOver() ? 0.5 : 0.0));
				f.draw();

				if (f.region().leftClicked()) setSelectedAsset(*it);

				moveDrawPos(0, 15);
			}
			moveDrawPos(-16, 0);
		}
	}

	void setSelectedAsset(const shared_ptr<Asset>& asset);
};

