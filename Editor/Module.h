#pragma once

#include "Part.h"
#include "Viewer.h"

#include "ViewerManager.h"
#include "AssetManager.h"

class ModuleViewer;

class ModuleModel
	: public PartModel
{
public:
	void	MakeViewers() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const;
	void	Save(ptree& pt) const override { AddToJSON(pt); }
};

class ModuleViewer
	: public PartViewer
{
public:
	TextEditState		m_textEditState_name;
	TextEditState		m_textEditState_mass;

public:
	ModuleViewer(const shared_ptr<PartModel>& model)
		: PartViewer(model)
		, m_textEditState_name(Unicode::Widen(model->GetName()))
		, m_textEditState_mass(ToString(model->m_mass))
	{
		m_drawRect = RectF(0, 450, 600, 150);
	}

	void	Update() override;
};

inline void ModuleModel::MakeViewers()
{
	g_viewerManagerPtr->MakeViewer<ModuleViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

inline void ModuleModel::SetFromJSON(const ptree& pt)
{
	PartModel::SetFromJSON(pt);
}

inline void ModuleModel::AddToJSON(ptree& pt) const
{
	PartModel::AddToJSON(pt);

	// type
	pt.put("type", "ModuleModel");
}

inline void ModuleViewer::Update()
{
	// name
	{
		SimpleGUI::TextBox(m_textEditState_name, Vec2(10, 10), 240);
		m_model->SetName(Unicode::Narrow(m_textEditState_name.text));
	}

	// mass
	{
		SimpleGUI::TextBox(m_textEditState_mass, Vec2(10, 50), 240);
		m_model->m_mass = Parse<double>(m_textEditState_mass.text);
	}
}
