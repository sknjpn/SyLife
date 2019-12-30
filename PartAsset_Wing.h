#pragma once

#include "PartAsset.h"

class PartAsset_Wing
	: public PartAsset
{
	bool	m_isRight;
	double	m_strength;

public:
	shared_ptr<PartState>	makeState() override;

	// Get
	bool	getIsRight() const { return m_isRight; }
	double	getStrength() const { return m_strength; }

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
	
	// TypeName
	String	getTypeName() override { return U"PartAsset_Wing"; }
};
