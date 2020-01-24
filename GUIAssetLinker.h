#pragma once

#include "EasyViewer.h"

class Asset;

class GUIAssetLinker 
	: public EasyViewer
{
	shared_ptr<Asset>	m_asset;
	function<void(const shared_ptr<Asset>&)>	m_functionOnChanged;
	
public:
	class Dragger
		: public EasyViewer
	{
		shared_ptr<Asset>	m_asset;

	public:
		Dragger(const shared_ptr<Asset>& asset)
			: m_asset(asset)
		{}

		void	init() override;
		void	update() override;

		const shared_ptr<Asset>& getAsset() const { return m_asset; }
	};

public:
	GUIAssetLinker(shared_ptr<Asset> asset = nullptr)
		: m_asset(asset)
	{}
	GUIAssetLinker(function<void(const shared_ptr<Asset>&)> functionOnChanged, shared_ptr<Asset> asset = nullptr)
		: m_asset(asset)
		, m_functionOnChanged(functionOnChanged)
	{}

	void	init() override;
	void	update() override;
};

