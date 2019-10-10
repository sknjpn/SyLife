#pragma once

#include "Viewer.h"

class CellAsset;

class ReleaseViewer
	: public Viewer
{
	bool	m_isDragged;

public:
	ReleaseViewer();

	void	setCellAsset(const shared_ptr<CellAsset> cellAsset) { m_cellAsset = cellAsset; }
	
	bool	isDragged() const { return m_isDragged; }

	void	update() override;
};

