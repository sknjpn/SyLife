﻿#pragma once

#include "Part.h"

class ModuleModel
	: public PartModel
{
public:
	void MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	load_this(const ptree& pt);
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
	void	Draw(const CellState& cell) const { GetPartConfig()->GetModel()->Draw(); }
	void	Update(CellState& cell) override {}
};

class ModuleViewer
	: public PartViewer
{
public:
	// Reload
	void ReloadProperties_this()
	{
		PartViewer::ReloadProperties_this();
	}
	void ReloadProperties() override { ReloadProperties_this(); }

	// Update
	void Update_this()
	{
		PartViewer::Update_this();
	}
	void Update() override { Update_this(); }
};

inline void		ModuleModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<ModuleViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}

inline shared_ptr<PartState>	ModuleModel::MakeState() { return make_shared<ModuleState>(); }

inline void ModuleModel::load_this(const ptree& pt)
{
	PartModel::load_this(pt);
}
