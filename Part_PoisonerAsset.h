#pragma once

#include "PartAsset.h"

class Part_PoisonerAsset
	: public PartAsset
{
	double	m_amount;
	double	m_interval;

public:
	std::shared_ptr<PartState>	makePartState(const std::shared_ptr<PartConfig>& partConfig) const override;

	// Get
	double	getAmount() const { return m_amount; }
	double	getInterval() const { return m_interval; }

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;

	bool	isPreRenderOnStateEnabled() const override { return true; }
	bool	isDrawOnStateEnabled() const override { return false; }

	// TypeName
	String	getTypeName() override { return U"Part_PoisonerAsset"; }
};
