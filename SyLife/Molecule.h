#pragma once

#include "Model.h"
#include "Viewer.h"
#include "Rigidbody.h"
#include "ViewerManager.h"

class MoleculeModel
	: public Model
{
	double		m_mass;
	double		m_radius;
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
		Model::AddToJSON(pt);

		// type
		pt.put("type", "Storage");
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

	void Update(bool isMouseOver) override {}
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
