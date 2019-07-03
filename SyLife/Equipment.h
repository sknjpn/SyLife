#pragma once

#include "Part.h"

class EquipmentModel
	: public PartModel
{
public:
	void MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	Load_this(const ptree& pt);
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const
	{
		return PartModel::Save_this(pt);

		// type
		pt.put("type", "EquipmentModel");
	}
	void	Save(ptree& pt) const override { Save_this(pt); }
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
	EquipmentViewer()
	{
		SetDrawRect(0, 450, 600, 150);
	}

public:
	void Update() override {}
};

inline void		EquipmentModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<EquipmentViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}
inline shared_ptr<PartState>	EquipmentModel::MakeState() { return make_shared<EquipmentState>(); }

inline void EquipmentModel::Load_this(const ptree& pt)
{
	PartModel::Load_this(pt);
}
