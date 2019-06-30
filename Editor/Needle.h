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
	s3d::TextEditState		m_textEditState_name;
	s3d::TextEditState		m_textEditState_mass;

public:
	NeedleViewer(const shared_ptr<PartModel>& model)
		: PartViewer(model)
		, m_textEditState_name(s3d::Unicode::Widen(model->GetName()))
		, m_textEditState_mass(s3d::ToString(model->m_mass))
	{
		m_drawRect = s3d::RectF(0, 450, 600, 150);
	}
};

inline void NeedleModel::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<NeedleModel>(dynamic_pointer_cast<PartModel>(shared_from_this()));
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
