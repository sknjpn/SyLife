#pragma once

#include "Part.h"

class BodyModel
	: public PartModel
{
public:
	shared_ptr<Viewer>		MakeViewer() override;
	shared_ptr<PartState>	MakeState() override;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const
	{
		PartModel::AddToJSON(pt);

		// type
		pt.put("type", "BodyModel");
	}
	void	Save(ptree& pt) const override { AddToJSON(pt); }
};

class BodyState
	: public PartState
{
public:
	void	Draw(const CellState& cell) const { GetPartConfig()->GetModel()->Draw(); }
	void	Update(CellState& cell) override {}
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
		, m_textEditState_mass(ToString(model->GetMass()))
	{
		m_drawRect = RectF(0, 450, 600, 150);
	}

	void Update(bool isMouseOver) override
	{
		// name
		{
			SimpleGUI::TextBox(m_textEditState_name, Vec2(10, 10), 240);
			m_model->SetName(Unicode::Narrow(m_textEditState_name.text));
		}

		// mass
		{
			SimpleGUI::TextBox(m_textEditState_mass, Vec2(10, 50), 240);
			m_model->m_mass =(Parse<double>(m_textEditState_mass.text));
		}
	}
};

inline shared_ptr<Viewer> BodyModel::MakeViewer() { return g_viewerManagerPtr->MakeViewer<BodyViewer>(dynamic_pointer_cast<PartModel>(shared_from_this())); }
inline shared_ptr<PartState>	BodyModel::MakeState() { return make_shared<BodyState>(); }

inline void BodyModel::SetFromJSON(const ptree& pt)
{
	PartModel::SetFromJSON(pt);
}
