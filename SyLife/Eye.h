#pragma once

#include "Equipment.h"

class EyeModel
	: public EquipmentModel
{
public:
	shared_ptr<PartConfig>	MakeConfig() override;
	shared_ptr<PartState>	MakeState() override;
};

class EyeConfig
	: public EquipmentConfig
{
public:
};

class EyeState
	: public EquipmentState
{
public:
	void	Draw(const Cell& cell) const { m_config->m_model->Draw(); }
	void	Update(Cell& cell) override {}
};

inline shared_ptr<PartConfig>	EyeModel::MakeConfig() { return make_shared<EyeConfig>(); }
inline shared_ptr<PartState>	EyeModel::MakeState() { return make_shared<EyeState>(); }
