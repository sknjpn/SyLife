#pragma once

#include "Module.h"

class ChloroplastModel
	: public ModuleModel
{
public:
	void		makeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	load_this(const ptree& pt) { ModuleModel::load_this(pt); }
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const
	{
		ModuleModel::save_this(pt);

		// type
		pt.put("type", "ChloroplastModel");
	}
	void	save(ptree& pt) const override { save_this(pt); }
};

class ChloroplastState
	: public ModuleState
{
public:
	void	Draw(const CellState& cell) const { GetPartConfig()->getModel()->Draw(); }
	void	Update(CellState& cell) override;

	void	MakeNutrition();
};

class ChloroplastViewer
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
