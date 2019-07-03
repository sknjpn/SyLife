#pragma once

#include "Equipment.h"

class EyeModel
	: public EquipmentModel
{
public:
	void MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	Load_this(const ptree& pt);
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const
	{
		EquipmentModel::Save_this(pt);

		// type
		pt.put("type", "EyeModel");
	}
	void	Save(ptree& pt) const override { Save_this(pt); }
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

inline void EyeModel::Load_this(const ptree& pt)
{
	EquipmentModel::Load_this(pt);
}