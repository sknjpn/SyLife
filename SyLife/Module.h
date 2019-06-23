#pragma once

#include "Part.h"

class ModuleModel
	: public PartModel
{
public:
	shared_ptr<PartConfig>	MakeConfig() override;
	shared_ptr<PartState>	MakeState() override;

	void	SetFromJSON(const ptree& pt);

	string	GetFilepath() const override { return "assets/part/" + GetFilename(); }

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class ModuleConfig
	: public PartConfig
{
public:
	shared_ptr<ModuleModel>	m_model;

public:
	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class ModuleState
	: public PartState
{
public:
	void	Draw(const Cell& cell) const { m_config->m_model->Draw(); }
	void	Update(Cell& cell) override {}
};

inline shared_ptr<PartConfig>	ModuleModel::MakeConfig() { return make_shared<ModuleConfig>(); }
inline shared_ptr<PartState>	ModuleModel::MakeState() { return make_shared<ModuleState>(); }
