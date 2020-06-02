#pragma once

#include "EasyViewer.h"

class Asset;

class Asset_Editor : public EasyViewer
{
	std::shared_ptr<Asset> m_asset;

	bool	m_isChanged = false;

public:
	Asset_Editor(const std::shared_ptr<Asset>& asset)
		: m_asset(asset)
	{}

	bool	setIsChanged() { m_isChanged = true; }
	
	template<typename T>
	std::shared_ptr<T>	getAsset() const { return std::dynamic_pointer_cast<T>(m_asset); }

	void	init() override;
	void	update() override;

	virtual void	apply() const;
};
