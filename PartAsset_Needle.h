#pragma once

#include "PartAsset.h"

class PartAsset_Needle
	: public PartAsset
{
	int		m_penetrating;

public:
	// State
	shared_ptr<PartState>	makePartState(const shared_ptr<PartConfig>& partConfig) const override;

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;

	int	getPenetrating() const { return m_penetrating; }

	// TypeName
	String	getTypeName() override { return U"PartAsset_Needle"; }
};
