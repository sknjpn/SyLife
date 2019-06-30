#pragma once

#include "Part.h"
#include "Viewer.h"

#include "ViewerManager.h"
#include "AssetManager.h"

class EquipmentViewer;

class EquipmentModel
	: public PartModel
{
public:
	void	MakeViewers() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const;
	void	Save(ptree& pt) const override { AddToJSON(pt); }
};


class EquipmentViewer
	: public PartViewer
{
public:
	s3d::TextEditState		m_textEditState_name;
	s3d::TextEditState		m_textEditState_mass;

public:
	EquipmentViewer(const shared_ptr<PartModel>& model)
		: PartViewer(model)
		, m_textEditState_name(s3d::Unicode::Widen(model->GetName()))
		, m_textEditState_mass(s3d::ToString(model->m_mass))
	{
		m_drawRect = s3d::RectF(0, 450, 600, 150);
	}

public:
	void	Update() override;
};

inline void EquipmentModel::MakeViewers()
{
	g_viewerManagerPtr->MakeViewer<EquipmentViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

inline void EquipmentModel::SetFromJSON(const ptree& pt)
{
	PartModel::SetFromJSON(pt);
}

inline void EquipmentModel::AddToJSON(ptree& pt) const
{
	return PartModel::AddToJSON(pt);

	// type
	pt.put("type", "EquipmentModel");
}

inline void EquipmentViewer::Update()
{
	// name
	{
		s3d::SimpleGUI::TextBox(m_textEditState_name, s3d::Vec2(10, 10), 240);
		m_model->SetName(s3d::Unicode::Narrow(m_textEditState_name.text));
	}

	// mass
	{
		s3d::SimpleGUI::TextBox(m_textEditState_mass, s3d::Vec2(10, 50), 240);
		m_model->m_mass = s3d::Parse<double>(m_textEditState_mass.text);
	}
}
