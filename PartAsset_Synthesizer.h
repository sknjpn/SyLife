#pragma once

#include "PartAsset.h"

class PartAsset_Synthesizer
	: public PartAsset
{
	std::shared_ptr<ProteinAsset>	m_export;
	double	m_productTime;

public:
	// State
	std::shared_ptr<PartState>	makePartState(const std::shared_ptr<PartConfig>& partConfig) const override;

	// Get
	const std::shared_ptr<ProteinAsset>& getExport() const { return m_export; }
	double	getProductTime() const { return m_productTime; }

	// JSON
	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;

	void	preRender(Image& image, double scale, const std::shared_ptr<PartConfig>& partConfig, double a = 0.5) const override {}
	
	// TypeName
	String	getTypeName() override { return U"PartAsset_Synthesizer"; }
};
