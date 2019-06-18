#pragma once

#include "Model.h"
#include "ShapeModelModel.h"

class PartModel
	: public Model
{
public:
	double	m_mass;
	vector<ShapeModel>	m_shapes;

public:
};
