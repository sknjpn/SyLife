#pragma once

#include "Viewer.h"

class CellAsset;

class ReleaseViewer
	: public Viewer
{
	shared_ptr<CellAsset>	m_cellAsset;
	bool	m_isDragged;

public:
	ReleaseViewer();

	void	setCellAsset(const shared_ptr<CellAsset> cellAsset) { m_cellAsset = cellAsset; }
	
	bool	isDragged() const { return m_isDragged; }

	void	update() override;
};

