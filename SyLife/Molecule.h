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
	void	MakeViewer() override;

	// Get
	double	GetMass() const { return m_mass; }
	double	GetRadius() const { return m_radius; }
	const Color&	GetColor() const { return m_color; }

	// JSON
	void	Load_this(const ptree& pt)
	{
		// mass
		m_mass = pt.get<double>("mass");

		// color
		m_color = Color(pt.get<int>("color.r"), pt.get<int>("color.g"), pt.get<int>("color.b"));

		// radius
		m_radius = sqrt(m_mass);

		Model::Load_this(pt);
	}
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const
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
	void	Save(ptree& pt) const override { Save_this(pt); }
	string	GetFilepath() const override { return "assets/models/molecules/" + GetFilename(); }
};

class MoleculeViewer
	: public ModelViewer
{
	TextEditState		m_textEditState_mass;

public:
	// Reload
	void ReloadProperties_this() 
	{
		ModelViewer::ReloadProperties_this();

		m_textEditState_mass.text = ToString(GetModel<MoleculeModel>()->m_mass); 
	}
	void ReloadProperties() override { ReloadProperties_this(); }

	// Update
	void Update_this()
	{
		ModelViewer::Update_this();

		// mass
		SimpleGUI::TextBox(m_textEditState_mass, Vec2(8,8));
		GetModel<MoleculeModel>()->m_mass = Parse<double>(m_textEditState_mass.text);
		MoveDrawPos(0, 48);
	}
	void Update() override { Update_this(); }
};

inline void MoleculeModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<MoleculeViewer>()->SetModel(shared_from_this());
}
