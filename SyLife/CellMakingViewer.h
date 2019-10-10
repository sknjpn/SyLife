#pragma once

#include "Viewer.h"

class CellAsset;

class CellMakingViewer :
	public Viewer
{
	bool	m_isOpened;
	shared_ptr<CellAsset>	m_cellAsset;

public:
	void	init() override;
	void	update() override;
};

