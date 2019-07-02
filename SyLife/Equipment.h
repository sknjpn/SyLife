#pragma once

#include "Part.h"

class EquipmentModel
	: public PartModel
{
public:
	shared_ptr<Viewer> MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const
	{
		return PartModel::AddToJSON(pt);

		// type
		pt.put("type", "EquipmentModel");
	}
	void	Save(ptree& pt) const override { AddToJSON(pt); }
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
	TextEditState		m_textEditState_name;
	TextEditState		m_textEditState_mass;

public:
	EquipmentViewer(const shared_ptr<PartModel>& model)
		: PartViewer(model)
		, m_textEditState_name(Unicode::Widen(model->GetName()))
		, m_textEditState_mass(ToString(model->GetMass()))
	{
		m_drawRect = RectF(0, 450, 600, 150);
	}

public:
	void Update(bool isMouseOver) override {}
};

inline shared_ptr<Viewer> EquipmentModel::MakeViewer(){return g_viewerManagerPtr->MakeViewer<EquipmentViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));}

inline shared_ptr<PartState>	EquipmentModel::MakeState() { return make_shared<EquipmentState>(); }

inline void EquipmentModel::SetFromJSON(const ptree& pt)
{
	PartModel::SetFromJSON(pt);
}
