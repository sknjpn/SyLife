#pragma once

#include "Viewer.h"

class CellAsset;
class BodyAsset;
class FieldViewer;

class CellMakingViewer :
	public Viewer
{
	friend class AssemblyViewer;
	friend class ReleaseViewer;
	friend class PartPaletteViewer;
	friend class ShapeLayerViewer;
	friend class ShapeAssemblyViewer;

	enum struct Mode
	{
		EditParts,
		EditBodyShapes,
	} m_mode;

	shared_ptr<CellAsset>	m_cellAsset;
	shared_ptr<BodyAsset>	m_bodyAsset;

public:
	void	update() override;
	void	init() override;

	void	setMode(Mode mode);

	void	makeAsset();

	const shared_ptr<CellAsset>& getCellAsset() const { return m_cellAsset; }
};

