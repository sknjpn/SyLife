#pragma once

#include "Equipment.h"
#include "Cell.h"

class NeedleModel
	: public EquipmentModel
{
public:
	void	makeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	load_this(const ptree& pt) { EquipmentModel::load_this(pt); }
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const
	{
		EquipmentModel::save_this(pt);

		// type
		pt.put("type", "NeedleModel");
	}
	void	save(ptree& pt) const override { save_this(pt); }
};

class NeedleState
	: public EquipmentState
{
	double	m_heat = 0.0;

public:
	void	Draw(const CellState& cell) const;
	void	Update(CellState& cell) override;
};

class NeedleViewer
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
