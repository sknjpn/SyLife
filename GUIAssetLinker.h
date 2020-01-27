#pragma once

#include "EasyViewer.h"

class Asset;

class GUIAssetLinker 
	: public EasyViewer
{
	std::shared_ptr<Asset>	m_asset;
	std::function<void(const std::shared_ptr<Asset>&)>	m_functionOnChanged;
	
public:
	class Dragger
		: public EasyViewer
	{
		std::shared_ptr<Asset>	m_asset;

	public:
		Dragger(const std::shared_ptr<Asset>& asset)
			: m_asset(asset)
		{}

		void	init() override
		{
			setViewerRectInLocal(RectF(50, 50).setCenter(Cursor::PosF()));
		}

		void	update() override
		{
			if (MouseL.up()) destroy();

			RectF(getViewerSize()).rounded(5).draw(ColorF(0.5, 0.5)).drawFrame(2.0, 0.0, Palette::Black);

			{
				const auto t = Transformer2D(Mat3x2::Scale(50));

				m_asset->drawIcon();
			}

			// 最後に位置は修正
			setViewerRectInLocal(RectF(50, 50).setCenter(getViewerPosInWorld() + Cursor::PosF()));
		}


		const std::shared_ptr<Asset>& getAsset() const { return m_asset; }
	};

public:
	GUIAssetLinker(std::shared_ptr<Asset> asset = nullptr)
		: m_asset(asset)
	{}
	GUIAssetLinker(std::function<void(const std::shared_ptr<Asset>&)> functionOnChanged, std::shared_ptr<Asset> asset = nullptr)
		: m_asset(asset)
		, m_functionOnChanged(functionOnChanged)
	{}

	void	init() override
	{

	}

	void	update() override
	{
		RectF(getViewerSize()).rounded(5)
			.draw(Palette::White)
			.drawFrame(2.0, 0.0, Palette::Black);

		{
			const auto t = Transformer2D(Mat3x2::Scale(Min(getViewerSize().x, getViewerSize().y)));

			m_asset->drawIcon();
		}

		if (const auto dragger = GetRootViewer()->getChildViewer<Dragger>())
		{
			if (MouseL.up() && RectF(getViewerSize()).rounded(5).mouseOver())
			{
				m_asset = dragger->getAsset();
				m_functionOnChanged(dragger->getAsset());
			}

			RectF(getViewerSize()).rounded(5)
				.draw(ColorF(0.5, 0.5))
				.drawFrame(2.0, 0.0, Palette::Black);
		}
	}
};

