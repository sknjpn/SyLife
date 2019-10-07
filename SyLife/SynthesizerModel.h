#pragma once

#include "ModuleModel.h"
#include "Storage.h"
#include "Cell.h"

class SynthesizerModel
	: public ModuleModel
{
	Storage	m_import;
	shared_ptr<MoleculeModel>	m_export;

public:
	const Storage& GetImport() const { return m_import; }
	const shared_ptr<MoleculeModel>& GetExport() const { return m_export; }

	void	makeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	void	save(ptree& pt) const override { save_this(pt); }
};
