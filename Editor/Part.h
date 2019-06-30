#pragma once

#include "Model.h"
#include "Viewer.h"
#include "Storage.h"
#include "ShapeModel.h"

#include "ViewerManager.h"
#include "AssetManager.h"

class PartViewer;
class PartConfig;

class PartModel
	: public Model
{
public:
	double	m_mass;
	Storage	m_material;
	vector<ShapeModel>	m_shapes;

public:
	void	MakeViewers() override;

	s3d::RectF	GetApproximateRect() const;
	double		GetRectInertia() const;
	s3d::Vec2	GetCenter() const;

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	AddToJSON(ptree& pt) const;
	void	Save(ptree& pt) const override { AddToJSON(pt); }

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

	void	AddToJSON(ptree& pt) const;
	void	SetFromJSON(const ptree& pt);

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
	void	Save(ptree& pt) const override { AddToJSON(pt); }
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

inline void PartModel::MakeViewers()
{
	g_viewerManagerPtr->MakeViewer<PartViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

inline s3d::RectF PartModel::GetApproximateRect() const
{
	s3d::RectF ar(m_shapes.front().m_verticles.front(), m_shapes.front().m_verticles.front());

	for (const auto& s : m_shapes)
	{
		for (const auto& v : s.m_verticles)
		{
			if (ar.x > v.x) { ar.w += ar.x - v.x; ar.x = v.x; }
			if (ar.y > v.y) { ar.h += ar.y - v.y; ar.y = v.y; }
			if (ar.br().x < v.x) ar.w = v.x - ar.x;
			if (ar.br().y < v.y) ar.h = v.y - ar.y;
		}
	}

	return ar;
}

inline double PartModel::GetRectInertia() const
{
	double w = GetApproximateRect().w;
	double h = GetApproximateRect().h;

	return  m_mass * (w * w + h * h) / 12.0;
}

inline s3d::Vec2 PartModel::GetCenter() const
{
	return GetApproximateRect().center();
}

inline void PartModel::SetFromJSON(const ptree& pt)
{
	// mass
	m_mass = pt.get<double>("mass");

	// shapes
	for (auto shape : pt.get_child("shapes"))
		m_shapes.emplace_back().Load(shape.second);

	// material
	m_material.Load(pt.get_child("material"));

	Model::SetFromJSON(pt);
}

inline void PartModel::AddToJSON(ptree& pt) const
{
	// mass
	pt.put<double>("mass", m_mass);

	// shapes
	{
		ptree shapes;

		for (const auto& v : m_shapes)
		{
			ptree shape; v.Save(shape);
			shapes.push_back(std::make_pair("", shape));
		}

		pt.add_child("shapes", shapes);
	}

	// material
	{
		ptree material;

		m_material.Save(material);

		pt.add_child("material", material);
	}

	Model::AddToJSON(pt);

	pt.put("type", "PartModel");
}

inline void PartConfig::SetFromJSON(const ptree& pt)
{
	// model
	m_model = g_assetManagerPtr->GetModel<PartModel>(pt.get<string>("model"));

	// position
	m_position = s3d::Vec2(pt.get<double>("position.x"), pt.get<double>("position.y"));

	// rotation
	m_rotation = pt.get<double>("rotation");

	Model::SetFromJSON(pt);
}

inline void PartConfig::AddToJSON(ptree& pt) const
{
	// model
	pt.put("model", m_model->GetName());

	// position
	{
		ptree position;

		position.put("x", m_position.x);
		position.put("y", m_position.y);

		pt.push_back(std::make_pair("position", position));
	}

	// rotation
	pt.put("rotation", m_rotation);

	Model::AddToJSON(pt);

	pt.put("type", "PartConfig");
}

inline void PartViewer::Update()
{

}
