#pragma once

#include "Part.h"

class BodyModel : public PartModel
{
public:
	Shape		m_shape;
	double		m_inertia;
};

class Body : public Part
{
public:

public:
	Body();
	~Body();
};

