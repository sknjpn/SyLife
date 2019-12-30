#pragma once

#include "PartAsset.h"

class PartAsset_Synthesizer
	: public PartAsset
{
	shared_ptr<ProteinAsset>	m_export;
	double	m_productTime;

public:
	// State
	shared_ptr<PartState>	makeState() override;

	// Get
	const shared_ptr<ProteinAsset>& getExport() const { return m_export; }
	double	getProductTime() const { return m_productTime; }

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
	
	// TypeName
	String	getTypeName() override { return U"PartAsset_Synthesizer"; }
};
