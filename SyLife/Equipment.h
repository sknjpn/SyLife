#pragma once

#include "Part.h"

class Equipment : public Part
{
public:
	double		m_rotation;
	Vector2D	m_position;

public:
	Equipment();
	~Equipment();
};

