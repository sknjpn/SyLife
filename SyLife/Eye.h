#pragma once

#include "Equipment.h"

class EyeModel
	: public EquipmentModel
{
public:
	void MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	load_this(const ptree& pt);
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
	void ReloadProperties_this()
	{
		EquipmentViewer::ReloadProperties_this();
	}
	void ReloadProperties() override { ReloadProperties_this(); }

	// Update
	void Update_this()
	{
		EquipmentViewer::Update_this();
	}
	void Update() override { Update_this(); }
};

inline void		EyeModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<EyeViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}

inline shared_ptr<PartState>	EyeModel::MakeState() { return make_shared<EyeState>(); }

inline void EyeModel::load_this(const ptree& pt)
{
	EquipmentModel::load_this(pt);
}