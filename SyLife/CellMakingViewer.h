#pragma once

#include "Viewer.h"

#include "AssemblyViewer.h"
#include "ReleaseViewer.h"
#include "PartPaletteViewer.h"
#include "ShapeAssemblyViewer.h"
#include "ShapeLayerViewer.h"

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

	AssemblyViewer		m_assemblyViewer;
	PartPaletteViewer	m_partPaletteViewer;
	ShapeLayerViewer	m_shapeLayerViewer;
	ShapeAssemblyViewer	m_shapeAssemblyViewer;

	shared_ptr<CellAsset>	m_cellAsset;
	shared_ptr<BodyAsset>	m_bodyAsset;

public:
	CellMakingViewer()
	{
		// 新しいモデルの登録
		makeAsset();

		setMode(Mode::EditParts);
	}

	void	update() override;

	void	setMode(Mode mode);

	void	makeAsset();

	const shared_ptr<CellAsset>& getCellAsset() const { return m_cellAsset; }
};

