#pragma once

#include "Viewer.h"
#include "CellModel.h"

class CellViewer :
	public Viewer
{
public:
	shared_ptr<CellModel>	m_model;

public:
	CellViewer(const shared_ptr<CellModel>& model)
		: m_model(model)
	{
		m_drawRect = s3d::RectF(0, 0, 600, 600);
	}

	void	Update() override;
};

