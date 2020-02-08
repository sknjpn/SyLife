#pragma once

#include "PartAsset.h"

class PartAsset_Needle
	: public PartAsset
{
	int		m_penetrating;

public:
	// State
	std::shared_ptr<PartState>	makePartState(const std::shared_ptr<PartConfig>& partConfig) const override;

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;

	bool	isPreRenderOnStateEnabled() const override { return false; }
	bool	isDrawOnStateEnabled() const override { return true; }

	int	getPenetrating() const { return m_penetrating; }

	// TypeName
	String	getTypeName() override { return U"PartAsset_Needle"; }
};
