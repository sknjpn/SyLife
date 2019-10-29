#pragma once

#include "Viewer.h"

class CellAsset;

class CellMakingViewer :
	public Viewer
{
	enum struct Mode
	{
		EditParts,
		EditBodyShapes,
	} m_mode;

	shared_ptr<CellAsset>	m_cellAsset;

public:
	void	update() override;
	void	init() override;

	void	setMode(Mode mode);

	void	makeAsset();

	const shared_ptr<CellAsset>& getCellAsset() const { return m_cellAsset; }
};

