#pragma once

#include "Viewer.h"
#include "DraggableViewer.h"

#include "TinyCamera.h"

struct Layer;
class PartAsset;
class PartAsset_Body;
class CellAsset;
class CellState;
class PartConfig;

class MainViewer : public Viewer
{
	class CellBook : public Viewer
	{
		class Item : public Viewer
		{
			shared_ptr<CellAsset> m_cellAsset;

		public:
			Item(const shared_ptr<CellAsset>& cellAsset);

			void	init() override;
			void	update() override;
		};

	public:
		void	addItem(const shared_ptr<CellAsset>& cellAsset);

		void	init() override;
		void	update() override;
	};

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
				shared_ptr<PartAsset_Body>	m_bodyAsset;

				Polygon	getReversed(const Polygon& polygon) const;
				Layer& getSelectedLayer();

			public:
				Workspace(const shared_ptr<PartAsset_Body>& bodyAsset)
					: m_bodyAsset(bodyAsset)
				{}

				void	init() override;
				void	update() override;

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
					shared_ptr<PartAsset_Body>	m_bodyAsset;

				public:
					Item(const shared_ptr<PartAsset_Body>& bodyAsset);

					void	setSelected(bool isSelected) { m_isSelected = isSelected; }
					void	setHSV(const HSV& hsv) { m_hsv = hsv; }
					void	update() override;
					const HSV& getHSV() const { return m_hsv; }
				};

				double	m_itemHeight = 100;
				int		m_selectedIndex;
				shared_ptr<PartAsset_Body>	m_bodyAsset;

			public:
				LayerLists(const shared_ptr<PartAsset_Body>& bodyAsset)
					: m_bodyAsset(bodyAsset)
				{}

				void	init() override;
				void	update() override;

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
				class TrashBox : public Viewer
				{
					bool	m_isSelected = false;
					Texture	m_textureTrashBox = Texture(Icon(0xf1f8, 80));

				public:
					void	select() { m_isSelected = true; }

					void	init() override;
					void	update() override;
				};

				enum struct State
				{
					MoveMode,
					RotateMode,
				};

				shared_ptr<CellAsset> m_cellAsset;

				State	m_state = State::MoveMode;
				Vec2	m_deltaPosition = Vec2::Zero();
				double	m_deltaRotation = 0;

				double	m_mass;
				double	m_inertia;
				double	m_radius;
				bool	m_grabCircle;
				shared_ptr<PartConfig>	m_selectedPartConfig;

			public:
				void	init() override;
				void	update() override;

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
		void	release();

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

