#pragma once

#include "Part.h"

class BodyModel
	: public PartModel
{
public:
	shared_ptr<PartState>	MakeState() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class BodyState
	: public PartState
{
public:
	void	Draw(const CellState& cell) const { m_config->m_model->Draw(); }
	void	Update(CellState& cell) override {}
};

inline shared_ptr<PartState>	BodyModel::MakeState() { return make_shared<BodyState>(); }

inline void BodyModel::SetFromJSON(const ptree& pt)
{
	PartModel::SetFromJSON(pt);
}
