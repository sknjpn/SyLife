#pragma once

#include "Part.h"

class ModuleModel
	: public PartModel
{
public:
	void MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	Load_this(const ptree& pt);
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const
	{
		PartModel::Save_this(pt);

		// type
		pt.put("type", "ModuleModel");
	}
	void	Save(ptree& pt) const override { Save_this(pt); }
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

inline void ModuleModel::Load_this(const ptree& pt)
{
	PartModel::Load_this(pt);
}
