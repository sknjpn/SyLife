#pragma once

#include "Model.h"
#include "Viewer.h"
#include "ShapeModel.h"

class PartViewer;
class PartConfig;

class PartModel
	: public Model
{
public:
	double		m_mass;
	vector<ShapeModel>	m_shapes;

public:
	void	MakeViewers() override;
	virtual shared_ptr<PartConfig>	MakeConfig() const;

	s3d::RectF	GetApproximateRect() const;
	double		GetRectInertia() const;
	s3d::Vec2	GetCenter() const;

	ptree	AddToJSON(ptree pt) const;
	void	SetFromJSON(const ptree& pt);

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	Save() const override { return AddToJSON(ptree()); }

	string	GetFilepath() const override { return "assets/parts/" + GetFilename(); }
};

class PartConfig
	: public Model
{
public:
	shared_ptr<PartModel>	m_model;
	s3d::Vec2	m_position;
	double		m_rotation;

public:
	double		GetInertia() const { return m_model->GetRectInertia() + (m_position + m_model->GetCenter().rotated(m_rotation)).lengthSq() * m_model->m_mass; }

	ptree	AddToJSON(ptree pt) const;
	void	SetFromJSON(const ptree& pt);

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	Save() const override { return AddToJSON(ptree()); }
};

class PartViewer
	: public Viewer
{
public:
	shared_ptr<PartModel>	m_model;

public:
	PartViewer(const shared_ptr<PartModel>& model)
		: m_model(model)
	{
		m_drawRect = s3d::RectF(0, 450, 600, 150);
	}

	void	Update() override;
};

inline shared_ptr<PartConfig> PartModel::MakeConfig() const { return make_shared<PartConfig>(); }
