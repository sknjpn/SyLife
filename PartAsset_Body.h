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
	double	m_scale;

	// State
	std::shared_ptr<PartState>	makePartState(const std::shared_ptr<PartConfig>& partConfig) const override;

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;

	void	draw(double a) override;
	
	// TypeName
	String	getTypeName() override { return U"PartAsset_Body"; }
};
