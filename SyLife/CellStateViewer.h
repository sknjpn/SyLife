#pragma once

#include "Viewer.h"

class CellState;

class CellStateViewer
	: public Viewer
{
public:
	shared_ptr<CellState>	m_cellState;

public:
	CellStateViewer();

	void	update() override;
};

