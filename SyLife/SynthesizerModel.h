#pragma once

#include "ModuleModel.h"

class SynthesizerModel
	: public ModuleModel
{
	shared_ptr<MoleculeModel>	m_product;

public:
	const shared_ptr<MoleculeModel>& GetProduct() const { return m_product; }

	void	makeViewer() override;
	shared_ptr<PartState>	makeState() override;

	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	void	save(ptree& pt) const override { save_this(pt); }
};
