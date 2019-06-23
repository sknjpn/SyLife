#pragma once

#include "Module.h"

class ChloroplastModel
	: public ModuleModel
{
public:
	shared_ptr<PartConfig>	MakeConfig() override;
	shared_ptr<PartState>	MakeState() override;
};

class ChloroplastConfig
	: public ModuleConfig
{
public:
};

class ChloroplastState
	: public ModuleState
{
public:
	void	Draw(const Cell& cell) const { m_config->m_model->Draw(); }
	void	Update(Cell& cell) override {}

	void	MakeNutrition();
};

inline shared_ptr<PartConfig>	ChloroplastModel::MakeConfig() { return make_shared<ChloroplastConfig>(); }
inline shared_ptr<PartState>	ChloroplastModel::MakeState() { return make_shared<ChloroplastState>(); }
