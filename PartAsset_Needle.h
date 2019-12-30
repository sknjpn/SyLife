#pragma once

#include "PartAsset.h"

class PartAsset_Needle
	: public PartAsset
{
	double	m_damage;

public:
	// State
	shared_ptr<PartState>	makePartState(const shared_ptr<PartConfig>& partConfig) const override;

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
	
	double	getDamage() const { return m_damage; }

	// TypeName
	String	getTypeName() override { return U"PartAsset_Needle"; }
};
