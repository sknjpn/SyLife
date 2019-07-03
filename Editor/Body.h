#pragma once

#include "Part.h"
#include "Viewer.h"

#include "ViewerManager.h"
#include "AssetManager.h"

class BodyViewer;

class BodyModel
	: public PartModel
{
public:
	void	MakeViewers() override;

	void	Load_this(const ptree& pt);
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const;
	void	Save(ptree& pt) const override { Save_this(pt); }
};

class BodyViewer
	: public PartViewer
{
public:
	TextEditState		m_textEditState_name;
	TextEditState		m_textEditState_mass;

public:
	BodyViewer(const shared_ptr<PartModel>& model)
		: PartViewer(model)
		, m_textEditState_name(Unicode::Widen(model->GetName()))
		, m_textEditState_mass(ToString(model->m_mass))
	{
		m_drawRect = RectF(0, 450, 600, 150);
	}

	void	Update() override;
};

inline void BodyModel::MakeViewers()
{
	g_viewerManagerPtr->MakeViewer<BodyViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

inline void BodyModel::Load_this(const ptree& pt)
{
	PartModel::Load_this(pt);
}

inline void BodyModel::Save_this(ptree& pt) const
{
	PartModel::Save_this(pt);

	// type
	pt.put("type", "BodyModel");
}

inline void BodyViewer::Update()
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
