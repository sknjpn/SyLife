#pragma once

#include "AssetModel.h"
#include "ModelViewer.h"
#include "Rigidbody.h"
#include "Storage.h"

class MoleculeModel
	: public AssetModel
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

class MoleculeViewer
	: public ModelViewer
{
	TextEditState		m_textEditState_mass;

public:
	// Reload
	void reloadProperties_this() 
	{
		ModelViewer::reloadProperties_this();

		m_textEditState_mass.text = ToString(GetModel<MoleculeModel>()->m_mass); 
	}
	void reloadProperties() override { reloadProperties_this(); }

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
