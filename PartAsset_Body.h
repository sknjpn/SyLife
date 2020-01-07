#pragma once

#include "PartAsset.h"

class PartAsset_Body
	: public PartAsset
{
public:
	// State
	shared_ptr<PartState>	makePartState(const shared_ptr<PartConfig>& partConfig) const override;

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
	
	// TypeName
	String	getTypeName() override { return U"PartAsset_Body"; }
};
