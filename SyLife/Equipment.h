#pragma once

#include "Part.h"

class EquipmentModel
	: public PartModel
{
public:
	shared_ptr<PartConfig>	MakeConfig() override;
	shared_ptr<PartState>	MakeState() override;

	string	GetFilepath() const override { return "assets/part/" + GetFilename(); }

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class EquipmentConfig
	: public PartConfig
{
public:
	shared_ptr<EquipmentModel>	m_model;

public:
	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class EquipmentState
	: public PartState
{
public:
	void	Draw(const Cell& cell) const { m_config->m_model->Draw(); }
	void	Update(Cell& cell) override {}
};

inline shared_ptr<PartConfig>	EquipmentModel::MakeConfig() { return make_shared<EquipmentConfig>(); }
inline shared_ptr<PartState>	EquipmentModel::MakeState() { return make_shared<EquipmentState>(); }
