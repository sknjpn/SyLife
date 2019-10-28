#pragma once

#include "Viewer.h"

#include "AssemblyViewer.h"
#include "ReleaseViewer.h"
#include "PartPaletteViewer.h"
#include "ShapeAssemblyViewer.h"
#include "ShapeLayerViewer.h"

class CellAsset;
class BodyAsset;

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
		Close,
		EditParts,
		EditBodyShapes,
		Release,
	} m_mode;

	AssemblyViewer		m_assemblyViewer;
	ReleaseViewer		m_releaseViewer;
	PartPaletteViewer	m_partPaletteViewer;
	ShapeLayerViewer	m_shapeLayerViewer;
	ShapeAssemblyViewer	m_shapeAssemblyViewer;

	shared_ptr<CellAsset>	m_cellAsset;
	shared_ptr<BodyAsset>	m_bodyAsset;

public:
	CellMakingViewer()
	{
		setPriority(1);

		// サブViewer生成
		//m_assemblyViewer = g_viewerManagerPtr->makeViewer<AssemblyViewer>();
		//m_partPaletteViewer = g_viewerManagerPtr->makeViewer<PartPaletteViewer>();
		//m_releaseViewer = g_viewerManagerPtr->makeViewer<ReleaseViewer>();
		//m_shapeAssemblyViewer = g_viewerManagerPtr->makeViewer<ShapeAssemblyViewer>();
		//m_shapeLayerViewer = g_viewerManagerPtr->makeViewer<ShapeLayerViewer>();

		// 新しいモデルの登録
		makeAsset();

		setMode(Mode::Close);
	}

	void	update() override;

	void	setMode(Mode mode);

	void	makeAsset();
};

