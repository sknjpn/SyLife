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
	{
		m_drawRect = s3d::RectF(0, 450, 600, 150);
	}

	void	Update() override;
};

