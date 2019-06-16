#pragma once

#include "Viewer.h"

#include "Body.h"

class BodyPropertyViewer :
	public Viewer
{
public:
	shared_ptr<BodyModel>	m_model;

public:
	void	Update() override;
};

