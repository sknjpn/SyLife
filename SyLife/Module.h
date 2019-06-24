#pragma once

#include "Part.h"

class ModuleModel
	: public PartModel
{
public:
	
	shared_ptr<PartState>	MakeState() override;

	void	SetFromJSON(const ptree& pt);

	string	GetFilepath() const override { return "assets/part/" + GetFilename(); }

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class ModuleState
	: public PartState
{
public:
	void	Draw(const Cell& cell) const { m_config->m_model->Draw(); }
	void	Update(Cell& cell) override {}
};

inline shared_ptr<PartState>	ModuleModel::MakeState() { return make_shared<ModuleState>(); }

inline void ModuleModel::SetFromJSON(const ptree& pt)
{
	PartModel::SetFromJSON(pt);
}
