#pragma once

#include "Viewer.h"

class Asset;

class EditorViewer
	: public Viewer
{
public:
	class AssetList
		: public Viewer
	{
		shared_ptr<Asset>	m_selectedAsset;

	public:
		class Item
			: public Viewer
		{
			shared_ptr<Asset>	m_asset;
			bool	m_isSelected = false;
			bool	m_isGrabbed = false;

		public:
			Item(const shared_ptr<Asset>& asset)
				: m_asset(asset)
			{}

			void	init() override;
			void	update() override;

			const shared_ptr<Asset>& getAsset() const { return m_asset; }
		};

	public:
		void	init() override;
		void	update() override;

		void	updateItemRects() const;

		void	setSelectedAsset(const shared_ptr<Asset>& asset) { m_selectedAsset = asset; }
		const shared_ptr<Asset>& getSelectedAsset() const { return m_selectedAsset; }
	};

	class AssetViewer
		: public Viewer
	{

	};

	void	init() override;
	void	update() override;
};

