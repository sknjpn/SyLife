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
		void	onDestroy() override;

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
				double	m_scale = 4.0;
				DynamicTexture	m_texture;
				std::shared_ptr<PartAsset_Body>	m_bodyAsset;

				Polygon	getStamp() const;
				Polygon	getStampOnImage() const { return getStamp().scaled(m_scale).movedBy(getViewerSize() / 2.0); }
				Polygon	getStampReversed() const;
				Polygon	getStampOnImageReversed() const { return getStampReversed().scaled(m_scale).movedBy(getViewerSize() / 2.0); }

				const Color& getColor() const;

			public:
				Workspace(const std::shared_ptr<PartAsset_Body>& bodyAsset)
					: m_bodyAsset(bodyAsset)
				{}

				void	onDestroy() override;
				void	init() override;
				void	update() override;

				void	detach(const Polygon& polygon);
			};

		public:
			class ColorSelector : public EasyViewer
			{
				const Array<Color> m_colors = { Palette::Blue, Palette::Purple, Palette::Red, Palette::Pink, Palette::Orange, Palette::Yellow, Palette::Yellowgreen, Palette::Green, Palette::Skyblue, Palette::Wheat,Palette::Black, Palette::Brown };
				Color	m_selectedColor = Palette::Blue;
				double	m_timer = 1.0;
				bool	m_eraseMode = false;
				std::shared_ptr<PartAsset_Body>	m_bodyAsset;

			public:
				const Color& getSelectedColor() const { return m_selectedColor; }
				bool	isEraseMode() const { return m_eraseMode; }

				void	init() override;
				void	update() override;
			};

			bool	m_isSymmetrical;
			double	m_scale;

		public:
			void	init() override;
			void	update() override;

			bool	isSymmetrical() const { return m_isSymmetrical; }
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
	void	setHiddenMode();
	void	unsetHiddenMode();

public:
	void	init() override;
	void	update() override;

	void	addCellAssetViewer(const std::shared_ptr<CellAsset>& cellAsset);
	void	addCellAssetViewer(const std::shared_ptr<CellState>& cellState);
};

