#pragma once

#include "Part.h"

class EquipmentModel : public PartModel
{
public:
	ShapeModel		m_shape;
	double		m_inertia;
};
