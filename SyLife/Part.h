#pragma once

#include "ShapeModel.h"

class Part
{
public:
	shared_ptr<PartModel>	m_model;

public:
	Part() {}
	virtual ~Part() {}

	virtual void	Draw() {}
	virtual void	Update() {}
};
