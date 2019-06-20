#pragma once

#include "Viewer.h"
#include "CellModel.h"
#include "TinyCamera.h"
#include "PartModel.h"

class AssemblyViewer :
	public Viewer
{
	shared_ptr<CellModel>	m_model;

	CursorCamera2D	m_camera;

public:
	AssemblyViewer(const shared_ptr<CellModel>& model)
		: m_model(model)
	{
		m_drawRect = s3d::RectF(0, 0, 450, 450);
	}

	void	Init() override;
	void	Update() override;
};
