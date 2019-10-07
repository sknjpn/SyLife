#pragma once

#include "Equipment.h"

class EyeModel
	: public EquipmentModel
{
public:
	void makeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	load_this(const ptree& pt) { EquipmentModel::load_this(pt); }
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const
	{
		EquipmentModel::save_this(pt);

		// type
		pt.put("type", "EyeModel");
	}
	void	save(ptree& pt) const override { save_this(pt); }
};

class EyeState
	: public EquipmentState
{
public:
	void	Draw(const CellState& cell) const { GetPartConfig()->GetModel()->Draw(); }
	void	Update(CellState& cell) override {}
};

class EyeViewer
	: public EquipmentViewer
{
public:
	// Reload
	void reloadProperties_this()
	{
		EquipmentViewer::reloadProperties_this();
	}
	void reloadProperties() override { reloadProperties_this(); }

	// Update
	void Update_this()
	{
		EquipmentViewer::Update_this();
	}
	void Update() override { Update_this(); }
};
