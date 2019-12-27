#pragma once

#include "PartAsset.h"

class PartAsset_Synthesizer
	: public PartAsset
{
	shared_ptr<ElementAsset>	m_export;

public:
	// State
	shared_ptr<PartState>	makeState() override;

	// Get
	const shared_ptr<ElementAsset>& getExport() const { return m_export; }

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
	
	// TypeName
	String	getTypeName() override { return U"PartAsset_Synthesizer"; }
};
