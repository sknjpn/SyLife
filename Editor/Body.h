#pragma once

#include "Part.h"
#include "Viewer.h"

class BodyViewer;
class BodyConfig;

class BodyModel
	: public PartModel
{
public:
	void	MakeViewers() override;
	shared_ptr<PartConfig>	MakeConfig() const override;

	ptree	AddToJSON(ptree pt) const;
	void	SetFromJSON(const ptree& pt);

	string	GetFilepath() const override { return "assets/part/" + GetFilename(); }

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	Save() const override { return AddToJSON(ptree()); }
};

class BodyConfig
	: public PartConfig
{
public:
	shared_ptr<BodyModel>	m_model;

public:
	ptree	AddToJSON(ptree pt) const;
	void	SetFromJSON(const ptree& pt);

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	Save() const override { return AddToJSON(ptree()); }
};

class BodyViewer
	: public PartViewer
{
public:
	s3d::TextEditState		m_textEditState_name;
	s3d::TextEditState		m_textEditState_mass;

public:
	BodyViewer(const shared_ptr<PartModel>& model)
		: PartViewer(model)
		, m_textEditState_name(s3d::Unicode::Widen(model->m_name))
		, m_textEditState_mass(s3d::ToString(model->m_mass))
	{
		m_drawRect = s3d::RectF(0, 450, 600, 150);
	}

	void	Update() override;
};

inline shared_ptr<PartConfig> BodyModel::MakeConfig() const { return make_shared<BodyConfig>(); }
