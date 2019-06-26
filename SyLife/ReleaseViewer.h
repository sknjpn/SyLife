#pragma once

#include "Viewer.h"
#include "Cell.h"

class ReleaseViewer
	: public Viewer
{
	shared_ptr<CellModel>	m_model;

public:
	ReleaseViewer(const shared_ptr<CellModel>& model)
		: m_model(model)
	{
		m_drawRect = s3d::RectF(s3d::Window::Width() - 800, 650, 600, 150);
	}

	void	Update(bool	isMouseOver) override;
};

