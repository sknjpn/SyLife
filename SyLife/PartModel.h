#pragma once

#include "Model.h"
#include "ShapeModel.h"

class PartModel
	: public Model
{
public:
	double	m_mass;
	vector<ShapeModel>	m_shapes;
	pair<Vector2D, Vector2D>	m_approximateRect;

public:
	void	Draw(double a = 0.5) { for (const auto& s : m_shapes) s.Draw(a); }

	double		GetRectInertia() const;
	Vector2D	GetCenter() const { return (m_approximateRect.first + m_approximateRect.second) / 2.0; }

	void	SetFromJSON(const ptree& pt);
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};
