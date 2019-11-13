#pragma once

#include "Viewer.h"
#include "TinyCamera.h"
#include "SlideBar.h"
#include "ColorPicker.h"

class PartAsset;
class CellAsset;
class PartConfig;

class CellMakingViewer :
	public Viewer
{

	class ShapeLayerViewer :
		public Viewer
	{
		int		m_selectedIndex = 0;
		double	m_itemHeight = 80;
		SlideBar	m_slideBar;
		ColorPicker	m_colorPicker;
		shared_ptr<PartAsset>	m_partAsset;

	public:
		ShapeLayerViewer();

		void	update() override;

		void	setPartAsset(const shared_ptr<PartAsset>& partAsset);

		void	drawLayers();

		int		getSelectedIndex() const { return m_selectedIndex; }
	};


	class ShapeAssemblyViewer
		: public Viewer
	{
		double	m_circleRadius = 10.0;
		Vec2	m_size;
		TinyCamera	m_camera;
		shared_ptr<PartAsset>	m_partAsset;

	public:
		ShapeAssemblyViewer()
		{
			setBackgroundColor(Palette::Gray);

			setViewerRect(RectF(800, 800).setCenter(getDrawCenter().movedBy(0, -50)));
			setSize(Vec2(800, 800));
		}

		void	update() override;

		void	setPartAsset(const shared_ptr<PartAsset>& partAsset) { m_partAsset = partAsset; }
		void	setSize(const Vec2& size);

		void	drawGrid() const;
	};


	class PartPaletteViewer
		: public Viewer
	{
		double	m_itemHeight = 80;
		shared_ptr<PartAsset>	m_selectedPart;

		Array<shared_ptr<PartAsset>>	getList() const;

	public:
		const shared_ptr<PartAsset>& getSelectedPart() const { return m_selectedPart; }

		void	clearSelectedPart() { m_selectedPart = nullptr; }

		void	drawAssets();

		void	init() override;
		void	update() override;
	};

	class AssemblyViewer
		: public Viewer
	{
		shared_ptr<CellAsset> m_cellAsset;

		enum struct State
		{
			MoveMode,
			RotateMode,
		} m_state = State::MoveMode;

		TinyCamera	m_camera;

		double	m_mass;
		double	m_inertia;
		double	m_radius;
		Vec2	m_size;
		bool	m_grabCircle;
		shared_ptr<PartConfig>	m_selectedPartConfig;

	public:
		AssemblyViewer();

		void	update() override;

		void	setSize(const Vec2& size);

		void	drawParts() const;
		void	drawGrid() const;

		void	setCellAsset(const shared_ptr<CellAsset>& cellAsset) { m_cellAsset = cellAsset; }
	};

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

