#pragma once

#include "Part.h"

class EquipmentModel
	: public PartModel
{
public:
	
	shared_ptr<PartState>	MakeState() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class EquipmentState
	: public PartState
{
public:
	void	Draw(const CellState& cell) const { GetPartConfig()->GetModel()->Draw(); }
	void	Update(CellState& cell) override {}
};

inline shared_ptr<PartState>	EquipmentModel::MakeState() { return make_shared<EquipmentState>(); }

inline void EquipmentModel::SetFromJSON(const ptree& pt)
{
	PartModel::SetFromJSON(pt);
}
