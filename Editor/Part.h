#pragma once

#include "Model.h"
#include "Viewer.h"
#include "ShapeModel.h"

#include "ViewerManager.h"
#include "AssetManager.h"

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

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	ptree	AddToJSON(ptree pt) const;
	ptree	Save() const override { return AddToJSON(ptree()); }

	string	GetFilepath() const override { return "assets/models/parts/" + GetFilename(); }
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


inline void PartModel::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<PartViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

inline void PartModel::SetFromJSON(const ptree & pt)
{
	// mass
	m_mass = pt.get<double>("mass");

	// shapes
	for (auto shape : pt.get_child("shapes"))
		m_shapes.emplace_back().SetFromJSON(shape.second);

	Model::SetFromJSON(pt);
}

inline ptree PartConfig::AddToJSON(ptree pt) const
{
	// model
	pt.put("name", m_model->m_name);

	// position
	{
		ptree position;

		position.put("x", m_position.x);
		position.put("y", m_position.y);

		pt.push_back(std::make_pair("position", position));
	}

	// rotation
	pt.put("rotation", m_rotation);

	return Model::AddToJSON(pt);
}

inline void PartConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetModel<PartModel>(pt.get<string>("name"));

	// position
	m_position = s3d::Vec2(pt.get<double>("position.x"), pt.get<double>("position.y"));

	// rotation
	m_rotation = pt.get<double>("rotation");

	Model::SetFromJSON(pt);
}
