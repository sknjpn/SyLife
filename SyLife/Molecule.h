#pragma once
/*
#include "Asset.h"
#include "ModelEditor.h"
#include "Rigidbody.h"
#include "Storage.h"

class MoleculeAsset
	: public Asset
{
	friend class MoleculeViewer;

	double	m_mass;
	double	m_radius;
	Color	m_color;
	Storage	m_material;

public:
	void	makeViewer() override;

	// Get
	double	getMass() const { return m_mass; }
	double	getRadius() const { return m_radius; }
	const Color&	getColor() const { return m_color; }

	// JSON
	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	void	save(ptree& pt) const override { save_this(pt); }
	string	getFilepath() const override { return "assets/models/molecules/" + getFilename(); }
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
	: public ModelEditor
{
	TextEditState		m_textEditState_mass;

public:
	// Reload
	void reloadProperties_this() 
	{
		ModelEditor::reloadProperties_this();

		m_textEditState_mass.text = ToString(getModel<MoleculeAsset>()->m_mass); 
	}
	void reloadProperties() override { reloadProperties_this(); }

	// update
	void Update_this()
	{
		ModelEditor::Update_this();

		// mass
		SimpleGUI::TextBox(m_textEditState_mass, Vec2(8,8));
		getModel<MoleculeAsset>()->m_mass = Parse<double>(m_textEditState_mass.text);
		MoveDrawPos(0, 48);
	}
	void update() override { Update_this(); }
};
*/