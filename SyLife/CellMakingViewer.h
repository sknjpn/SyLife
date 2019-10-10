#pragma once

#include "Viewer.h"

class CellAsset;

class CellMakingViewer :
	public Viewer
{
	shared_ptr<CellAsset>	m_cellAsset;

public:
	void	init() override;
	void	update() override;
};

