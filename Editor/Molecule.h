#pragma once

#include "Model.h"
#include "Viewer.h"
#include "ViewerManager.h"

class MoleculeViewer;

class MoleculeModel
	: public Model
{
	friend class MoleculeViewer;

	double		m_mass;
	double		m_radius;
	Color	m_color;

public:
	// Get
	double	GetMass() const { return m_mass; }
	double	GetRadius() const { return m_radius; }
	const Color&	GetColor() const { return m_color; }

	// Viewer
	void	MakeViewers() override;

	// JSON
	void	Load_this(const ptree& pt);
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const;
	void	Save(ptree& pt) const override { Save_this(pt); }
	string	GetFilepath() const override { return "assets/models/molecules/" + GetFilename(); }
};

class MoleculeViewer
	: public Viewer
{
	shared_ptr<MoleculeModel>	m_model;
	TextEditState		m_textEditState_name;
	TextEditState		m_textEditState_mass;

public:
	MoleculeViewer(const shared_ptr<MoleculeModel>& model)
		: m_model(model)
		, m_textEditState_name(Unicode::Widen(model->GetName()))
		, m_textEditState_mass(ToString(model->GetMass()))
	{
		m_drawRect = RectF(0, 0, 600, 600);
	}

	void	Update() override;
};

inline void MoleculeModel::MakeViewers()
{
	g_viewerManagerPtr->MakeViewer<MoleculeViewer>(dynamic_pointer_cast<MoleculeModel>(shared_from_this()));
}

inline void MoleculeModel::Save_this(ptree& pt) const
{
	// mass
	pt.put<double>("mass", m_mass);

	// color
	{
		ptree color;

		color.put("r", m_color.r);
		color.put("g", m_color.g);
		color.put("b", m_color.b);

		pt.add_child("color", color);
	}

	Model::Save_this(pt);

	pt.put("type", "MoleculeModel");
}

inline void MoleculeModel::Load_this(const ptree& pt)
{
	// mass
	m_mass = pt.get<double>("mass");

	// color
	m_color = Color(pt.get<int>("color.r"), pt.get<int>("color.g"), pt.get<int>("color.b"));

	// radius
	m_radius = sqrt(m_mass);

	Model::Load_this(pt);
}

inline void MoleculeViewer::Update()
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