#pragma once

#include "Model.h"
#include "ShapeModel.h"

class PartModel
	: public Model
{
public:
	double		m_mass;
	vector<ShapeModel>	m_shapes;

public:
	s3d::RectF	GetApproximateRect() const;
};
