#pragma once

#include "Model.h"
#include "ModelViewer.h"
#include "Rigidbody.h"

class MoleculeModel
	: public Model
{
	friend class MoleculeViewer;

	double	m_mass;
	double	m_radius;
	Color	m_color;

public:
	shared_ptr<Viewer>	MakeViewer() override;

	// Get
	double	GetMass() const { return m_mass; }
	double	GetRadius() const { return m_radius; }
	const Color&	GetColor() const { return m_color; }

	// JSON
	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const
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
	void	Save(ptree& pt) const override { AddToJSON(pt); }
};

class MoleculeState
	: public Rigidbody
{
	shared_ptr<MoleculeModel>	m_model;

public:
	// Get
	const shared_ptr<MoleculeModel>&	GetModel() const { return m_model; }

	// Set
	void	SetModel(const shared_ptr<MoleculeModel>& model)
	{
		m_model = model;
		SetRadius(model->GetRadius());
		SetMass(model->GetMass());
	}

	void	UpdateMolecule();
	void	Draw();
};

class MoleculeViewer
	: public ModelViewer
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
		SetDrawRect(0, 0, 600, 600);
	}

	void Update() override
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
};

inline shared_ptr<Viewer> MoleculeModel::MakeViewer() { return g_viewerManagerPtr->MakeViewer<MoleculeViewer>(dynamic_pointer_cast<MoleculeModel>(shared_from_this())); }

inline void MoleculeModel::SetFromJSON(const ptree& pt)
{
	// mass
	m_mass = pt.get<double>("mass");

	// color
	m_color = Color(pt.get<int>("color.r"), pt.get<int>("color.g"), pt.get<int>("color.b"));

	// radius
	m_radius = sqrt(m_mass);

	Model::SetFromJSON(pt);
}
