#pragma once

#include "EasyViewer.h"
#include "DraggableViewer.h"
#include "HiddenViewer.h"
#include "CellAsset.h"

#include "TinyCamera.h"

struct Layer;
class PartAsset;
class PartAsset_Body;
class CellAsset;
class CellState;
class PartConfig;

class MainViewer : public EasyViewer
{
	class CellAssetViewer : public DraggableViewer
	{
	public:
		class CellStateViewer : public DraggableViewer
		{
			std::shared_ptr<CellState>	m_cellState;

		public:
			CellStateViewer(const std::shared_ptr<CellState>& cellState);

			const std::shared_ptr<CellState>& getCellState() const { return m_cellState; }

			void	setCellState(const std::shared_ptr<CellState>& cellState) { m_cellState = cellState; }

			void	init() override;
			void	update() override;
		};
	private:

		std::shared_ptr<CellAsset>	m_cellAsset;
		std::shared_ptr<CellState>	m_cellState;

	public:
		CellAssetViewer(const std::shared_ptr<CellAsset>& cellAsset);
		CellAssetViewer(const std::shared_ptr<CellState>& cellState);

		void	setCellState(const std::shared_ptr<CellState>& cellState);

		void	init() override;
		void	update() override;

		double	getMax(const RectF& rect, int scale, std::function<double(const CellAsset::Log::Status&)> func) const;
		void	drawGraph(const RectF& rect, const Color& color, double max, int scale, std::function<double(const CellAsset::Log::Status&)> func) const;
	};

	class CellBook : public HiddenViewer
	{
		class Item : public EasyViewer
		{
			std::shared_ptr<CellAsset> m_cellAsset;

		public:
			Item(const std::shared_ptr<CellAsset>& cellAsset);

			void	init() override;
			void	update() override;
		};

	public:
		void	addItem(const std::shared_ptr<CellAsset>& cellAsset);

		void	init() override;
		void	update() override;
	};

	class FieldViewer : public EasyViewer
	{
		class CellStateCaptureViewer
			: public EasyViewer
		{
			std::shared_ptr<CellState> m_cellState;

		public:
			CellStateCaptureViewer(const std::shared_ptr<CellState>& cellState);

			void	init() override;
			void	update() override;
		};

		class ReleaseViewer
			: public EasyViewer
		{
			std::shared_ptr<CellAsset>	m_cellAsset;

		public:
			ReleaseViewer(const std::shared_ptr<CellAsset>& cellAsset);

			void	init() override;
			void	update() override;

			bool	isReleased() const { return !m_cellAsset; }
		};

		TinyCamera	m_camera;

		void	openCellMakingViewer();

	public:
		enum class HandAction {
			None,
			AddElement,
			Poison,
			Trash,
		} m_handAction;

		bool	m_isHighSpeed = false;
		bool	m_drawWaveEnabled = false;
		int		m_frameCount = 0;

		void	init() override;
		void	update() override;

		void	release(const std::shared_ptr<CellAsset>& cellAsset) { addChildViewer<ReleaseViewer>(cellAsset); m_handAction = HandAction::None; }

		TinyCamera& getCamera() { return m_camera; }
		const TinyCamera& getCamera() const { return m_camera; }
	};

	class CommandPalette : public HiddenViewer
	{
		Texture	m_textureZoomIn = Texture(Icon(0xf00e, 45));
		Texture	m_textureZoomOut = Texture(Icon(0xf010, 45));
		Texture	m_textureFast = Texture(Icon(0xf050, 45));
		Texture	m_textureWave = Texture(Icon(0xf5c4, 45));
		Texture	m_textureHand = Texture(Icon(0xf25a, 45));
		Texture	m_texturePoison = Texture(Icon(0xf714, 45));
		Texture m_textureAddElement = Texture(Icon(0xf613, 45));
		Texture	m_textureTrashBox = Texture(Icon(0xf1f8, 45));

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
				int		m_numCell = 0;
				int		m_numEgg = 0;

				Status() { }
				Status(const std::shared_ptr<CellAsset>& cellAsset);
			};

			std::shared_ptr<CellAsset>	m_cellAsset;
			Array<Status>	m_statuses;

			Log(const std::shared_ptr<CellAsset>& cellAsset)
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

	class CellMakingViewer : public EasyViewer
	{
		class BodySculptor : public EasyViewer
		{
			class Workspace : public EasyViewer
			{
				std::shared_ptr<PartAsset_Body>	m_bodyAsset;

				Polygon	getReversed(const Polygon& polygon) const;
				Layer& getSelectedLayer();

			public:
				Workspace(const std::shared_ptr<PartAsset_Body>& bodyAsset)
					: m_bodyAsset(bodyAsset)
				{}

				void	init() override;
				void	update() override;

				void	attach(const Polygon& polygon);
				void	detach(const Polygon& polygon);
			};

		public:
			class LayerLists : public EasyViewer
			{
				class Item : public EasyViewer
				{
					Texture	m_circleTexture;
					Texture	m_barTexture;
					bool	m_circleSelected = false;
					bool	m_barSelected = false;
					HSV		m_hsv = Palette::White;
					bool	m_isSelected = false;
					std::shared_ptr<PartAsset_Body>	m_bodyAsset;

				public:
					Item(const std::shared_ptr<PartAsset_Body>& bodyAsset);

					void	setSelected(bool isSelected) { m_isSelected = isSelected; }
					void	setHSV(const HSV& hsv) { m_hsv = hsv; }
					void	update() override;
					const HSV& getHSV() const { return m_hsv; }
				};

				double	m_itemHeight = 100;
				int		m_selectedIndex;
				std::shared_ptr<PartAsset_Body>	m_bodyAsset;

			public:
				LayerLists(const std::shared_ptr<PartAsset_Body>& bodyAsset)
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

		class PartsAssembler : public EasyViewer
		{
		public:
			class Workspace : public EasyViewer
			{
				class TrashBox : public EasyViewer
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

				std::shared_ptr<CellAsset> m_cellAsset;

				State	m_state = State::MoveMode;
				Vec2	m_deltaPosition = Vec2::Zero();
				double	m_deltaRotation = 0;

				double	m_mass;
				double	m_inertia;
				double	m_radius;
				bool	m_grabCircle;
				std::shared_ptr<PartConfig>	m_selectedPartConfig;

			public:
				void	init() override;
				void	update() override;

				void	setCellAsset(const std::shared_ptr<CellAsset>& cellAsset) { m_cellAsset = cellAsset; }

				void	setMoveMode();
				void	setRotateMode();
			};

			class PartList : public EasyViewer
			{
				double	m_itemHeight = 80;
				std::shared_ptr<PartAsset>	m_selectedPart;

				Array<std::shared_ptr<PartAsset>>	getList() const;

			public:
				void	init() override;
				void	update() override;

				const std::shared_ptr<PartAsset>& getSelectedPart() const { return m_selectedPart; }

				void	clearSelectedPart() { m_selectedPart = nullptr; }

				void	drawAssets();
			};

		public:
			void	init() override;
			void	update() override;
		};

		class CellInfo : public EasyViewer
		{
			TextEditState	m_textEditState_name;

		public:
			void	init() override;
			void	update() override;
		};


		std::shared_ptr<CellAsset>	m_cellAsset;

		void	clearEditor();
		void	openBodySculptor();
		void	openPartsAssembler();
		void	release();

	public:
		void	init() override;
		void	update() override;

		void	makeAsset();

		const std::shared_ptr<CellAsset>& getCellAsset() const { return m_cellAsset; }
	};

	Stopwatch	m_uncontrolTimer;
	bool	m_hiddenMode = false;

	void	openCellMakingViewer();

public:
	void	init() override;
	void	update() override;

	void	addCellAssetViewer(const std::shared_ptr<CellAsset>& cellAsset);
	void	addCellAssetViewer(const std::shared_ptr<CellState>& cellState);
};

