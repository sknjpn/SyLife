#pragma once

#include "Model.h"
#include "ShapeModel.h"

class Cell;
class PartConfig;
class PartState;

class PartModel
	: public Model
{
public:
	double	m_mass;
	vector<ShapeModel>	m_shapes;
	pair<Vector2D, Vector2D>	m_approximateRect;

public:
	virtual shared_ptr<PartState>	MakeState() = 0;

	void	Draw(double a = 0.5) { for (const auto& s : m_shapes) s.Draw(a); }

	double		GetRectInertia() const;
	Vector2D	GetCenter() const { return (m_approximateRect.first + m_approximateRect.second) / 2.0; }

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
	double		GetInertia() const { return m_model->GetRectInertia() + (m_position + m_model->GetCenter().rotated(m_rotation)).lengthSq() * m_model->m_mass; }

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

class PartState
{
public:
	shared_ptr<PartConfig>	m_config;

public:
	virtual ~PartState() {}

	virtual void	Draw(const Cell& cell) const = 0;
	virtual void	Update(Cell& cell) = 0;
};
