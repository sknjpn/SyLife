#pragma once

#include "Viewer.h"

class CellAsset;

class ReleaseViewer
	: public Viewer
{
public:
	shared_ptr<CellAsset>	m_cellAsset;
	bool	m_isDragged;

public:
	ReleaseViewer();

	void	update() override;
};

