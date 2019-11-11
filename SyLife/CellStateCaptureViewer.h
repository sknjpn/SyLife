#pragma once

#include "Viewer.h"

class CellState;

class CellStateCaptureViewer
	: public Viewer
{
	shared_ptr<CellState> m_cellState;

public:
	CellStateCaptureViewer(const shared_ptr<CellState>& cellState);

	void	update();
};

