#pragma once

#include "Part.h"

class BodyModel : public PartModel
{
public:
	ShapeModel		m_shape;
	double		m_inertia;
};
