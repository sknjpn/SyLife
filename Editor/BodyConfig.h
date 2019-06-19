#pragma once

#include "BodyModel.h"

class BodyConfig
{
public:
	shared_ptr<BodyModel>	m_model;

public:
	BodyConfig(const shared_ptr<BodyModel>& model)
		: m_model(model)
	{}
};

