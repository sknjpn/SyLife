#pragma once

#include "Module.h"

class ChloroplastModel
	: public ModuleModel
{
public:
	void		MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const
	{
		ModuleModel::AddToJSON(pt);

		// type
		pt.put("type", "ChloroplastModel");
	}
	void	Save(ptree& pt) const override { AddToJSON(pt); }
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
	ChloroplastViewer()
	{}
};

inline void		ChloroplastModel::MakeViewer() 
{
	g_viewerManagerPtr->MakeViewer<ChloroplastViewer>()->SetModel(shared_from_this()); 
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}
inline shared_ptr<PartState>	ChloroplastModel::MakeState() { return make_shared<ChloroplastState>(); }

inline void ChloroplastModel::SetFromJSON(const ptree& pt)
{
	ModuleModel::SetFromJSON(pt);
}

inline void ChloroplastState::MakeNutrition()
{
}
