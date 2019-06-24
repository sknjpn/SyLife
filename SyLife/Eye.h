#pragma once

#include "Equipment.h"

class EyeModel
	: public EquipmentModel
{
public:
	
	shared_ptr<PartState>	MakeState() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class EyeState
	: public EquipmentState
{
public:
	void	Draw(const Cell& cell) const { m_config->m_model->Draw(); }
	void	Update(Cell& cell) override {}
};

inline shared_ptr<PartState>	EyeModel::MakeState() { return make_shared<EyeState>(); }

inline void EyeModel::SetFromJSON(const ptree& pt)
{
	EquipmentModel::SetFromJSON(pt);
}