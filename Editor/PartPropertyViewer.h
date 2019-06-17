#pragma once

#include "Viewer.h"
#include "Part.h"

class PartPropertyViewer :
	public Viewer
{
public:
	shared_ptr<PartModel>	m_model;

public:
	PartPropertyViewer(const shared_ptr<PartModel>& model)
		: m_model(model)
	{}

	void	Update() override;
};

