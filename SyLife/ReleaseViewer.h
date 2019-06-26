#pragma once

#include "Viewer.h"
#include "Cell.h"

class ReleaseViewer
	: public Viewer
{
public:
	shared_ptr<CellModel>	m_model;
	bool	m_isDragged;

public:
	ReleaseViewer(const shared_ptr<CellModel>& model)
		: m_model(model)
	{
		m_drawRect = s3d::RectF(s3d::Window::Width() - 600, 450, 400, 100);
	}

	void	Update(bool	isMouseOver) override;
};

