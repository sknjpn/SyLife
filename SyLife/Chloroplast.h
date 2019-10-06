#pragma once

#include "Module.h"

class ChloroplastModel
	: public ModuleModel
{
public:
	void		MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	load_this(const ptree& pt);
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
	void	Draw(const CellState& cell) const { GetPartConfig()->GetModel()->Draw(); }
	void	Update(CellState& cell) override {}

	void	MakeNutrition();
};

class ChloroplastViewer
	: public ModuleViewer
{
public:
	// Reload
	void ReloadProperties_this()
	{
		ModuleViewer::ReloadProperties_this();
	}
	void ReloadProperties() override { ReloadProperties_this(); }

	// Update
	void Update_this()
	{
		ModuleViewer::Update_this();
	}
	void Update() override { Update_this(); }
};

inline void		ChloroplastModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<ChloroplastViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}
inline shared_ptr<PartState>	ChloroplastModel::MakeState() { return make_shared<ChloroplastState>(); }

inline void ChloroplastModel::load_this(const ptree& pt)
{
	ModuleModel::load_this(pt);
}

inline void ChloroplastState::MakeNutrition()
{
}
