#pragma once

#include "Viewer.h"
#include "TinyCamera.h"
#include "ColorPicker.h"

class PartAsset;
class CellAsset;
class PartConfig;

class CellMakingViewer
	: public Viewer
{
public:
	class BodySculptor
		: public Viewer
	{
	public:
		class Workspace
			: public Viewer
		{
			double	m_circleRadius = 10.0;
			Vec2	m_size;
			TinyCamera	m_camera;
			shared_ptr<PartAsset>	m_partAsset;

		public:
			void	init() override;
			void	update() override;

			void	setPartAsset(const shared_ptr<PartAsset>& partAsset) { m_partAsset = partAsset; }
			void	setSize(const Vec2& size);

			void	drawGrid() const;
		};

	public:
		class LayerLists
			: public Viewer
		{
			int		m_selectedIndex = 0;
			double	m_itemHeight = 80;
			ColorPicker	m_colorPicker;
			shared_ptr<PartAsset>	m_partAsset;

		public:
			void	init() override;
			void	update() override;

			void	setPartAsset(const shared_ptr<PartAsset>& partAsset);

			void	drawLayers();

			int		getSelectedIndex() const { return m_selectedIndex; }
		};

	public:
		void	init() override;
		void	update() override;
	};

	class PartsAssembler
		: public Viewer
	{
	public:
		class Workspace
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
			void	init() override;
			void	update() override;

			void	setSize(const Vec2& size);

			void	drawParts() const;
			void	drawGrid() const;

			void	setCellAsset(const shared_ptr<CellAsset>& cellAsset) { m_cellAsset = cellAsset; }
		};

		class PartList
			: public Viewer
		{
			double	m_itemHeight = 80;
			shared_ptr<PartAsset>	m_selectedPart;

			Array<shared_ptr<PartAsset>>	getList() const;

		public:
			void	init() override;
			void	update() override;

			const shared_ptr<PartAsset>& getSelectedPart() const { return m_selectedPart; }

			void	clearSelectedPart() { m_selectedPart = nullptr; }

			void	drawAssets();
		};

	public:
		void	init() override;
		void	update() override;
	};

	class CellInfo
		: public Viewer
	{

	public:
		void	init() override;
		void	update() override;
	};

private:
	shared_ptr<CellAsset>	m_cellAsset;

	void	clearEditor();
	void	openBodySculptor();
	void	openPartsAssembler();

public:
	void	init() override;
	void	update() override;

	void	makeAsset();

	const shared_ptr<CellAsset>& getCellAsset() const { return m_cellAsset; }
};

