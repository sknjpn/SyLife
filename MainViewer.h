#pragma once

#include "Viewer.h"
#include "DraggableViewer.h"

#include "TinyCamera.h"

struct Layer;
class PartAsset;
class CellAsset;
class CellState;
class PartConfig;

class MainViewer : public Viewer
{
	class FieldViewer : public Viewer
	{
		class CellStateCaptureViewer
			: public Viewer
		{
			shared_ptr<CellState> m_cellState;

		public:
			CellStateCaptureViewer(const shared_ptr<CellState>& cellState);

			void	update();
		};

		class ReleaseViewer
			: public Viewer
		{
			shared_ptr<CellAsset>	m_cellAsset;

		public:
			ReleaseViewer(const shared_ptr<CellAsset>& cellAsset);

			void	update() override;

			bool	isReleased() const { return !m_cellAsset; }
		};

		TinyCamera	m_camera;

		void	openCellMakingViewer();

	public:
		bool	m_isHighSpeed = false;
		int		m_frameCount = 0;

		void	init() override;
		void	update() override;

		void	release(const shared_ptr<CellAsset>& cellAsset) { addChildViewer<ReleaseViewer>(cellAsset); }

		TinyCamera& getCamera() { return m_camera; }
		const TinyCamera& getCamera() const { return m_camera; }
	};

	class CellStateViewer : public Viewer
	{
	public:
		shared_ptr<CellState>	m_cellState;

	public:
		CellStateViewer();

		void	update() override;
	};

	class CommandPalette : public Viewer
	{
		class MagnifyingViewer
			: public Viewer
		{
		public:
			void init() override;
			void update() override;
		};

		class SpeedControllerViewer : public Viewer
		{
			bool	m_isHighSpeed = false;
			int		m_updateCount = 0;

		public:
			void init() override;
			void update() override;

			bool isHighSpeed() const { return m_isHighSpeed; }
			void setUpdateCount(int updateCount) { m_updateCount = updateCount; }
		};

	public:
		void init() override;
		void update() override;
	};

	class StatisticsViewer : public DraggableViewer
	{
		struct Log
		{
			struct Status
			{
				int		m_num;

				Status(const shared_ptr<CellAsset>& cellAsset);
			};

			shared_ptr<CellAsset>	m_cellAsset;
			Array<Status>	m_statuses;

			Log(const shared_ptr<CellAsset>& cellAsset)
				: m_cellAsset(cellAsset)
			{}
		};

		Array<Log>	m_logs;
		int	m_statusesSizeMax = 1000;

	public:
		void	update() override;
		void	init() override;

		void	takeLog();
	};

	class CellMakingViewer : public Viewer
	{
		class BodySculptor : public Viewer
		{
			class Workspace : public Viewer
			{
				shared_ptr<PartAsset>	m_partAsset;

				Polygon	getReversed(const Polygon& polygon) const;
				Layer& getSelectedLayer();

			public:
				void	init() override;
				void	update() override;

				void	setPartAsset(const shared_ptr<PartAsset>& partAsset) { m_partAsset = partAsset; }

				void	attach(const Polygon& polygon);
				void	detach(const Polygon& polygon);
			};

		public:
			class LayerLists : public Viewer
			{
				class Item : public Viewer
				{
					Texture	m_circleTexture;
					Texture	m_barTexture;
					bool	m_circleSelected = false;
					bool	m_barSelected = false;
					HSV		m_hsv = Palette::White;
					bool	m_isSelected = false;
					shared_ptr<PartAsset>	m_partAsset;

				public:
					Item(const shared_ptr<PartAsset>& partAsset);

					void	setSelected(bool isSelected) { m_isSelected = isSelected; }
					void	update() override;
					const HSV& getHSV() const { return m_hsv; }
				};

				double	m_itemHeight = 100;
				shared_ptr<PartAsset>	m_partAsset;
				int		m_selectedIndex;

			public:
				void	init() override;
				void	update() override;

				void	setPartAsset(const shared_ptr<PartAsset>& partAsset);
				int		getSelectedIndex() const { return m_selectedIndex; }
			};

			enum struct State
			{
				Put,
				Shave,
			};

			bool	m_isSymmetrical;
			double	m_scale;
			State	m_state;

			void	setState(State state);

		public:
			void	init() override;
			void	update() override;

			bool	isSymmetrical() const { return m_isSymmetrical; }
			State	getState() const { return m_state; }
			double	getStampRadius() const;
		};

		class PartsAssembler : public Viewer
		{
		public:
			class Workspace : public Viewer
			{
				enum struct State
				{
					MoveMode,
					RotateMode,
				};

				shared_ptr<CellAsset> m_cellAsset;

				State	m_state = State::MoveMode;
				Vec2	m_prePosition;
				double	m_preRotation;

				double	m_mass;
				double	m_inertia;
				double	m_radius;
				bool	m_grabCircle;
				shared_ptr<PartConfig>	m_selectedPartConfig;

			public:
				void	init() override;
				void	update() override;

				void	drawParts() const;

				void	setCellAsset(const shared_ptr<CellAsset>& cellAsset) { m_cellAsset = cellAsset; }

				void	setMoveMode();
				void	setRotateMode();
			};

			class PartList : public Viewer
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

		class CellInfo : public Viewer
		{
			TextEditState	m_textEditState_name;

		public:
			void	init() override;
			void	update() override;
		};


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

	void	openCellMakingViewer();

public:
	void	init() override;
	void	update() override;
};

