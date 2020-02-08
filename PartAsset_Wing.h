#pragma once

#include "PartAsset.h"

class PartAsset_Wing
	: public PartAsset
{
	bool	m_isRight;
	double	m_strength;

public:
	std::shared_ptr<PartState>	makePartState(const std::shared_ptr<PartConfig>& partConfig) const override;

	// Get
	bool	getIsRight() const { return m_isRight; }
	double	getStrength() const { return m_strength; }

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;

	bool	drawOnAssetEnabled() const override { return false; }
	bool	drawOnStateEnabled() const override { return true; }

	// TypeName
	String	getTypeName() override { return U"PartAsset_Wing"; }
};
