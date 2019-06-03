#pragma once

#include "Shape.h"

class PartModel
{
public:
	string		m_name;
	double		m_mass;
};

class Part
{
public:
	shared_ptr<PartModel>	m_model;

public:
	Part();
	virtual ~Part() {}

	virtual void	Draw() {}
	virtual void	Update() {}
};
