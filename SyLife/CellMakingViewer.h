#pragma once

#include "Viewer.h"

class CellAsset;

class AssemblyViewer;
class ReleaseViewer;
class PartPaletteViewer;

class CellMakingViewer :
	public Viewer
{
	friend class AssemblyViewer;
	friend class ReleaseViewer;
	friend class PartPaletteViewer;

	bool	m_isOpened;
	bool	m_isReleasing;
	shared_ptr<CellAsset>	m_cellAsset;

public:
	void	init() override;
	void	update() override;

	void	open();
	void	close();

	void	release();
};

