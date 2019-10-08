#pragma once

#include "ModuleAsset.h"

class SynthesizerAsset
	: public ModuleAsset
{
	shared_ptr<MoleculeAsset>	m_product;

public:
	const shared_ptr<MoleculeAsset>& GetProduct() const { return m_product; }

	void	makeViewer() override;
	shared_ptr<PartState>	makeState() override;

	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	void	save(ptree& pt) const override { save_this(pt); }
};
