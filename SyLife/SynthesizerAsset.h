#pragma once

#include "ModuleAsset.h"

class SynthesizerAsset
	: public ModuleAsset
{
	shared_ptr<ElementAsset>	m_export;

public:
	const shared_ptr<ElementAsset>& getExport() const { return m_export; }

	void	makeViewer() override;
	shared_ptr<PartState>	makeState() override;

	void	load(const JSONValue& json) override;
	void	save(ptree& pt) const override;
};
