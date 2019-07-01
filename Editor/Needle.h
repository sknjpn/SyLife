#pragma once

#include "Equipment.h"
#include "Cell.h"

class NeedleViewer;

class NeedleModel
	: public EquipmentModel
{
public:
	void	MakeViewers() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const;
	void	Save(ptree& pt) const override { AddToJSON(pt); }
};

class NeedleViewer
	: public PartViewer
{
public:
	TextEditState		m_textEditState_name;
	TextEditState		m_textEditState_mass;

public:
	NeedleViewer(const shared_ptr<PartModel>& model)
		: PartViewer(model)
		, m_textEditState_name(Unicode::Widen(model->GetName()))
		, m_textEditState_mass(ToString(model->m_mass))
	{
		m_drawRect = RectF(0, 450, 600, 150);
	}
};

inline void NeedleModel::MakeViewers()
{
	g_viewerManagerPtr->MakeViewer<NeedleViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

inline void NeedleModel::SetFromJSON(const ptree& pt)
{
	EquipmentModel::SetFromJSON(pt);
}

inline void NeedleModel::AddToJSON(ptree& pt) const
{
	EquipmentModel::AddToJSON(pt);

	pt.put("type", "NeedleModel");
}
