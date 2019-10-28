#pragma once

#include "Viewer.h"

class CellAsset;
class BodyAsset;

class AssemblyViewer;
class ReleaseViewer;
class PartPaletteViewer;
class ShapeLayerViewer;
class ShapeAssemblyViewer;

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

	shared_ptr<AssemblyViewer>	m_assemblyViewer;
	shared_ptr<ReleaseViewer>	m_releaseViewer;
	shared_ptr<PartPaletteViewer>	m_partPaletteViewer;
	shared_ptr<ShapeLayerViewer>	m_shapeLayerViewer;
	shared_ptr<ShapeAssemblyViewer>	m_shapeAssemblyViewer;

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

