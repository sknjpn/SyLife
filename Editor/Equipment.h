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
	shared_ptr<PartConfig>	MakeConfig() const override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	AddToJSON(ptree pt) const;
	ptree	Save() const override { return AddToJSON(ptree()); }
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
		, m_textEditState_name(s3d::Unicode::Widen(model->m_name))
		, m_textEditState_mass(s3d::ToString(model->m_mass))
	{
		m_drawRect = s3d::RectF(0, 450, 600, 150);
	}

public:
	void	Update() override;
};

inline shared_ptr<PartConfig> EquipmentModel::MakeConfig() const { return make_shared<EquipmentConfig>(); }


inline void EquipmentModel::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<EquipmentViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

inline ptree EquipmentModel::AddToJSON(ptree pt) const
{
	// type
	pt.put("type", "Equipment");

	return PartModel::AddToJSON(pt);
}

inline void EquipmentModel::SetFromJSON(const ptree & pt)
{
	PartModel::SetFromJSON(pt);
}

inline void EquipmentViewer::Update()
{
	// name
	{
		s3d::SimpleGUI::TextBox(m_textEditState_name, s3d::Vec2(10, 10), 240);
		m_model->m_name = s3d::Unicode::Narrow(m_textEditState_name.text);
	}

	// mass
	{
		s3d::SimpleGUI::TextBox(m_textEditState_mass, s3d::Vec2(10, 50), 240);
		m_model->m_mass = s3d::Parse<double>(m_textEditState_mass.text);
	}
}
