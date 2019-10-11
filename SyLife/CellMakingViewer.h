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

	enum struct Mode
	{
		Close,
		EditParts,
		EditBodyShapes,
		Release,
	} m_mode;

	shared_ptr<CellAsset>	m_cellAsset;

public:
	void	init() override;
	void	update() override;

	void	open();
	void	close();

	void	release();
};

