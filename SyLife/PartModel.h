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
};
