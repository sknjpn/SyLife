#pragma once

#include "Equipment.h"

class EyeModel
	: public EquipmentModel
{
public:
	shared_ptr<Viewer> MakeViewer() override;
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
	EyeViewer(const shared_ptr<PartModel>& model)
		: EquipmentViewer(model)
	{}
};

inline shared_ptr<Viewer>		EyeModel::MakeViewer() { return g_viewerManagerPtr->MakeViewer<EyeViewer>(dynamic_pointer_cast<PartModel>(shared_from_this())); }
inline shared_ptr<PartState>	EyeModel::MakeState() { return make_shared<EyeState>(); }

inline void EyeModel::SetFromJSON(const ptree& pt)
{
	EquipmentModel::SetFromJSON(pt);
}