#pragma once

#include "Part.h"

class ModuleModel
	: public PartModel
{
public:
	void makeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	load_this(const ptree& pt) { PartModel::load_this(pt); }
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const
	{
		PartModel::save_this(pt);

		// type
		pt.put("type", "ModuleModel");
	}
	void	save(ptree& pt) const override { save_this(pt); }
};

class ModuleState
	: public PartState
{
public:
	void	Draw(const CellState& cell) const { GetPartConfig()->getModel()->Draw(); }
	void	Update(CellState& cell) override {}
};

class ModuleViewer
	: public PartEditor
{
public:
	// Reload
	void reloadProperties_this()
	{
		PartEditor::reloadProperties_this();
	}
	void reloadProperties() override { reloadProperties_this(); }

	// Update
	void Update_this()
	{
		PartEditor::Update_this();
	}
	void Update() override { Update_this(); }
};
