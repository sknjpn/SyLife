#pragma once

#include "Viewer.h"
#include "Part.h"

class PartPropertyViewer :
	public Viewer
{
public:
	shared_ptr<PartModel>	m_model;

public:
	void	Update() override;
};

