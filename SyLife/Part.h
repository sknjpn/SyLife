#pragma once

#include "Model.h"
#include "Storage.h"
#include "ShapeModel.h"
#include "AssetManager.h"

class CellState;
class PartConfig;
class PartState;
class MoleculeModel;

class PartModel
	: public Model
{
	friend class PartViewer;

	double	m_mass;
	Storage	m_material;
	vector<ShapeModel>	m_shapes;

public:
	// Get
	double	GetMass() const { return m_mass; }
	const Storage&	GetMaterial() const { return m_material; }
	const vector<ShapeModel>&	GetShapes() const { return m_shapes; }

	virtual shared_ptr<PartState>	MakeState() = 0;

	void	Draw(double a = 0.5) { for (const auto& s : m_shapes) s.Draw(a); }

	Vector2D	GetApproximateRectTopLeft() const;
	Vector2D	GetApproximateRectBottomDown() const;
	double		GetRectInertia() const;
	Vector2D	GetCenter() const { return (GetApproximateRectTopLeft() + GetApproximateRectBottomDown()) / 2.0; }

	// JSON
	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class PartConfig
	: public Model
{
	shared_ptr<PartModel>	m_partModel;
	Vector2D	m_position;
	double		m_rotation;

public:
	// Get
	const shared_ptr<PartModel>&	GetModel() const { return m_partModel; }
	const Vector2D&	GetPosition() const { return m_position; }
	double	GetRotation() const { return m_rotation; }

	// Set
	void	SetModel(const shared_ptr<PartModel>& partModel) { m_partModel = partModel; }
	void	SetPosition(const Vector2D& position) { m_position = position; }
	void	SetRotation(double rotation) { m_rotation = rotation; }

	double	GetInertia() const { return m_partModel->GetRectInertia() + (m_position + m_partModel->GetCenter().rotated(m_rotation)).lengthSq() * m_partModel->GetMass(); }

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class PartState
{
	shared_ptr<PartConfig>	m_partConfig;

public:
	virtual ~PartState() {}

	// Get
	const shared_ptr<PartConfig>&	GetPartConfig() const { return m_partConfig; }

	// Set
	void	SetPartConfig(const shared_ptr<PartConfig>& partConfig) { m_partConfig = partConfig; }

	virtual void	Draw(const CellState& cell) const = 0;
	virtual void	Update(CellState& cell) = 0;
};

inline Vector2D PartModel::GetApproximateRectTopLeft() const
{
	double x = m_shapes.front().m_polygon.vertices().front().x;
	double y = m_shapes.front().m_polygon.vertices().front().y;

	for (const auto& s : m_shapes)
	{
		for (const auto& v : s.m_polygon.vertices())
		{
			if (x > v.x) x = v.x;
			if (y > v.y) y = v.y;
		}
	}

	return Vector2D(x, y);
}

inline Vector2D PartModel::GetApproximateRectBottomDown() const
{
	double x = m_shapes.front().m_polygon.vertices().front().x;
	double y = m_shapes.front().m_polygon.vertices().front().y;

	for (const auto& s : m_shapes)
	{
		for (const auto& v : s.m_polygon.vertices())
		{
			if (x < v.x) x = v.x;
			if (y < v.y) y = v.y;
		}
	}

	return Vector2D(x, y);
}

inline double PartModel::GetRectInertia() const
{
	auto w = (GetApproximateRectBottomDown() - GetApproximateRectTopLeft()).m_x;
	auto h = (GetApproximateRectBottomDown() - GetApproximateRectTopLeft()).m_y;

	return  m_mass * (w * w + h * h) / 12.0;
}

inline void PartModel::SetFromJSON(const ptree& pt)
{
	// mass
	m_mass = pt.get<double>("mass");

	// shapes
	for (auto shape : pt.get_child("shapes"))
		m_shapes.emplace_back().SetFromJSON(shape.second);

	// material
	m_material.Load(pt.get_child("material"));

	Model::SetFromJSON(pt);
}

inline void PartConfig::SetFromJSON(const ptree& pt)
{
	// model
	m_partModel = g_assetManagerPtr->GetModel<PartModel>(pt.get<string>("model"));

	// position
	m_position = Vector2D(pt.get<double>("position.x"), pt.get<double>("position.y"));

	// rotation
	m_rotation = pt.get<double>("rotation");

	Model::SetFromJSON(pt);
}
