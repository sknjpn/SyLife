#pragma once

#include "PartAsset.h"

class PartAsset_Body
	: public PartAsset
{
public:
	PartAsset_Body()
		: m_image(800, 800)
	{}

	Texture	m_texture;
	Image	m_image;

	// State
	std::shared_ptr<PartState>	makePartState(const std::shared_ptr<PartConfig>& partConfig) const override;

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
	
	bool	drawOnAssetEnabled() const override { return true; }
	bool	drawOnStateEnabled() const override { return false; }

	void	draw(double a) override;
	void	preRender(Image& image, double scale, const std::shared_ptr<PartConfig>& partConfig, double a = 0.5) const override;
	
	// TypeName
	String	getTypeName() override { return U"PartAsset_Body"; }
};
