#pragma once

#include "Part.h"

class EquipmentModel
	: public PartModel
{
public:
	void MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const
	{
		PartModel::save_this(pt);

		// type
		pt.put("type", "EquipmentModel");
	}
	void	save(ptree& pt) const override { save_this(pt); }
};

class EquipmentState
	: public PartState
{
public:
	void	Draw(const CellState& cell) const { GetPartConfig()->GetModel()->Draw(); }
	void	Update(CellState& cell) override {}
};


class EquipmentViewer
	: public PartViewer
{
public:
	// Reload
	void reloadProperties_this()
	{
		PartViewer::reloadProperties_this();
	}
	void reloadProperties() override { reloadProperties_this(); }

	// Update
	void Update_this()
	{
		PartViewer::Update_this();
	}
	void Update() override { Update_this(); }
};

inline void		EquipmentModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<EquipmentViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}
inline shared_ptr<PartState>	EquipmentModel::MakeState() { return make_shared<EquipmentState>(); }

inline void EquipmentModel::load_this(const ptree& pt)
{
	PartModel::load_this(pt);
}
