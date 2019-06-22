#pragma once

#include "PartModel.h"

class PartState
{
public:
	shared_ptr<PartModel>	m_model;

public:
	PartState() {}
	virtual ~PartState() {}

	virtual void	Draw() {}
	virtual void	Update() {}
};
