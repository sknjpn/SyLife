#pragma once

#include "Module.h"
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

class SynthesizerState
	: public ModuleState
{
	double	m_timer = 0.0;

public:
	void	Draw(const CellState& cell) const
	{
		GetPartConfig()->GetModel()->Draw(min(m_timer / 2.0, 1.0) * 0.75 + 0.25);
	}

	void	Update(CellState& cell) override;
};

class SynthesizerViewer
	: public ModuleViewer
{
public:
	// Reload
	void reloadProperties_this()
	{
		ModuleViewer::reloadProperties_this();
	}
	void reloadProperties() override { reloadProperties_this(); }

	// Update
	void Update_this()
	{
		ModuleViewer::Update_this();
	}
	void Update() override { Update_this(); }
};
