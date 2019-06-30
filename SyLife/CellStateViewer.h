#pragma once

#include "Viewer.h"
#include "Cell.h"

class CellStateViewer
	: public Viewer
{
public:
	shared_ptr<CellState>	m_cellState;

public:
	CellStateViewer()
		: m_cellState(nullptr)
	{
		m_drawRect = s3d::RectF(s3d::Window::Width() - 300, 450, 200, 400);

	}

	void	Update(bool isMouseOver) override;
};

