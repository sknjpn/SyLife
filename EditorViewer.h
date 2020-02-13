#pragma once

#include "EasyViewer.h"

class Asset;

class EditorViewer : public EasyViewer
{
public:
	void	init() override;
	void	update() override;

	void	openAsset(const std::shared_ptr<Asset>& asset);

	class AssetViewer : public EasyViewer
	{
		std::shared_ptr<Asset> m_asset;

	public:
		void init() override;
		void update() override;

		const std::shared_ptr<Asset>& getAsset() const { return m_asset; }
	};

	class AssetList : public EasyViewer
	{
		class Item : public EasyViewer
		{
			std::shared_ptr<Asset>	m_asset;

		public:
			Item(const std::shared_ptr<Asset>& asset)
				: m_asset(asset)
			{}

			void init() override;
			void update() override;
		};

	public:
		void init() override;
		void update() override;

		void addAsset(const std::shared_ptr<Asset>& asset);
	};
};

