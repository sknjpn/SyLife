#pragma once

#include "Module.h"

class ChloroplastModel
	: public ModuleModel
{
public:
	
	shared_ptr<PartState>	MakeState() override;
};

class ChloroplastState
	: public ModuleState
{
public:
	void	Draw(const Cell& cell) const { m_config->m_model->Draw(); }
	void	Update(Cell& cell) override {}

	void	MakeNutrition();

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

inline shared_ptr<PartState>	ChloroplastModel::MakeState() { return make_shared<ChloroplastState>(); }

inline void ChloroplastModel::SetFromJSON(const ptree& pt)
{
	ModuleModel::SetFromJSON(pt);
}

inline void ChloroplastState::MakeNutrition()
{
}
