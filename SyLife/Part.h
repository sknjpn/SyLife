#pragma once

#include "Model.h"
#include "Storage.h"
#include "ShapeModel.h"

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
public:
	shared_ptr<PartModel>	m_model;
	Vector2D	m_position;
	double		m_rotation;

public:
	double		GetInertia() const { return m_model->GetRectInertia() + (m_position + m_model->GetCenter().rotated(m_rotation)).lengthSq() * m_model->GetMass(); }

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class PartState
{
public:
	shared_ptr<PartConfig>	m_config;

public:
	virtual ~PartState() {}

	virtual void	Draw(const CellState& cell) const = 0;
	virtual void	Update(CellState& cell) = 0;
};
