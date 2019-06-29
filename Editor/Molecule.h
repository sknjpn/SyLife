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
	s3d::Color	m_color;

public:
	// Get
	double	GetMass() const { return m_mass; }
	double	GetRadius() const { return m_radius; }
	const s3d::Color&	GetColor() const { return m_color; }

	// Viewer
	void	MakeViewers() override;

	// JSON
	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const;
	void	Save(ptree& pt) const override { AddToJSON(pt); }
	string	GetFilepath() const override { return "assets/models/molecules/" + GetFilename(); }
};

class MoleculeViewer
	: public Viewer
{
	shared_ptr<MoleculeModel>	m_model;
	s3d::TextEditState		m_textEditState_name;
	s3d::TextEditState		m_textEditState_mass;

public:
	MoleculeViewer(const shared_ptr<MoleculeModel>& model)
		: m_model(model)
		, m_textEditState_name(s3d::Unicode::Widen(model->GetName()))
		, m_textEditState_mass(s3d::ToString(model->GetMass()))
	{
		m_drawRect = s3d::RectF(0, 0, 600, 600);
	}

	void	Update() override;
};

inline void MoleculeModel::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<MoleculeViewer>(dynamic_pointer_cast<MoleculeModel>(shared_from_this()));
}

inline void MoleculeModel::AddToJSON(ptree& pt) const
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

	Model::AddToJSON(pt);

	pt.put("type", "MoleculeModel");
}

inline void MoleculeModel::SetFromJSON(const ptree& pt)
{
	// mass
	m_mass = pt.get<double>("mass");

	// color
	m_color = s3d::Color(pt.get<int>("color.r"), pt.get<int>("color.g"), pt.get<int>("color.b"));

	// radius
	m_radius = sqrt(m_mass);

	Model::SetFromJSON(pt);
}

inline void MoleculeViewer::Update()
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
