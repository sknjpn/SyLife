#pragma once

#include "Part.h"

class ModuleModel
	: public PartModel
{
public:
	void MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const
	{
		PartModel::AddToJSON(pt);

		// type
		pt.put("type", "ModuleModel");
	}
	void	Save(ptree& pt) const override { AddToJSON(pt); }
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
	ModuleViewer()
	{
		SetDrawRect(0, 450, 600, 150);
	}

	void Update() override {}
};

inline void		ModuleModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<ModuleViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}

inline shared_ptr<PartState>	ModuleModel::MakeState() { return make_shared<ModuleState>(); }

inline void ModuleModel::SetFromJSON(const ptree& pt)
{
	PartModel::SetFromJSON(pt);
}
