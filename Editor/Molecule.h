#pragma once

#include "Model.h"
#include "Viewer.h"

class MoleculeViewer;

class MoleculeModel
	: public Model
{
public:
	double		m_mass;
	double		m_radius;
	s3d::Color	m_color;

public:
	void	MakeViewers() override;

	ptree	AddToJSON(ptree pt) const;
	void	SetFromJSON(const ptree& pt);

	string	GetFilepath() const override { return "assets/models/molecules/" + GetFilename(); }

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	Save() const override { return AddToJSON(ptree()); }
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
		, m_textEditState_name(s3d::Unicode::Widen(model->m_name))
		, m_textEditState_mass(s3d::ToString(model->m_mass))
	{
		m_drawRect = s3d::RectF(0, 0, 600, 600);
	}

	void	Update() override;
};

