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

	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	void	save(ptree& pt) const override { save_this(pt); }
};
