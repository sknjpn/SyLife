#pragma once

#include "Part.h"

class EquipmentModel : public PartModel
{
public:
	Shape		m_shape;
	double		m_inertia;
};

class Equipment : public Part
{
public:
	double		m_rotation;
	Vector2D	m_position;

public:
	Equipment();
	~Equipment();
};

