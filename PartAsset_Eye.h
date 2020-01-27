#pragma once

#include "PartAsset.h"

class PartAsset_Eye
	: public PartAsset
{
	double	m_maxDistance;

public:
	// State
	std::shared_ptr<PartState>	makePartState(const std::shared_ptr<PartConfig>& partConfig) const override;

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;

	double	getMaxDistance() const { return m_maxDistance; }

	// TypeName
	String	getTypeName() override { return U"PartAsset_Eye"; }
};
