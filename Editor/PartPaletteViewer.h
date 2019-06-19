#pragma once

#include "Viewer.h"

#include "CellModel.h"

class PartPaletteViewer :
	public Viewer
{
public:
	shared_ptr<CellModel>	m_model;

public:
	PartPaletteViewer(const shared_ptr<CellModel>& model)
		: m_model(model)
	{
		m_drawRect = s3d::RectF(450, 0, 150, 450);
	}

	void	Update() override;
};

