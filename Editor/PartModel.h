#pragma once

#include "Model.h"
#include "ShapeModel.h"

class PartModel
	: public Model
{
public:
	double	m_mass;
	vector<Shape>	m_shapes;

public:
};
