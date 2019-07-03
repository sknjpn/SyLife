#pragma once

#include "Equipment.h"

class EyeModel
	: public EquipmentModel
{
public:
	void MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const
	{
		EquipmentModel::AddToJSON(pt);

		// type
		pt.put("type", "EyeModel");
	}
	void	Save(ptree& pt) const override { AddToJSON(pt); }
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
	EyeViewer()
	{}
};

inline void		EyeModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<EyeViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}

inline shared_ptr<PartState>	EyeModel::MakeState() { return make_shared<EyeState>(); }

inline void EyeModel::SetFromJSON(const ptree& pt)
{
	EquipmentModel::SetFromJSON(pt);
}