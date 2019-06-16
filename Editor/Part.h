#pragma once

#include "Model.h"
#include "Shape.h"

class PartModel
	: public Model
{
public:
	double	m_mass;
	vector<Shape>	m_shapes;

public:
};
