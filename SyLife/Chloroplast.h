#pragma once

#include "Module.h"

class ChloroplastModel
	: public ModuleModel
{
public:
	
	shared_ptr<PartState>	MakeState() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class ChloroplastState
	: public ModuleState
{
public:
	void	Draw(const CellState& cell) const { GetPartConfig()->GetModel()->Draw(); }
	void	Update(CellState& cell) override {}

	void	MakeNutrition();
};

inline shared_ptr<PartState>	ChloroplastModel::MakeState() { return make_shared<ChloroplastState>(); }

inline void ChloroplastModel::SetFromJSON(const ptree& pt)
{
	ModuleModel::SetFromJSON(pt);
}

inline void ChloroplastState::MakeNutrition()
{
}
