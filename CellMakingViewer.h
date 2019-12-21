#pragma once

#include "Viewer.h"
#include "Layer.h"
#include "GridViewer.h"
#include "ColorPicker.h"

class PartAsset;
class CellAsset;
class PartConfig;

class CellMakingViewer
	: public Viewer
{
	shared_ptr<CellAsset>	m_cellAsset;

public:
	class BodySculptor
		: public Viewer
	{
		bool	m_isSymmetrical;
		double	m_scale;
		Polygon	m_stamp;

	public:
		class Workspace
			: public Viewer
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

		bool	isSymmetrical() const { return m_isSymmetrical; }
		Polygon getStamp() const;
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
		TextEditState	m_textEditState_name;

	public:
		void	init() override;
		void	update() override;
	};

public:
	void	clearEditor();
	void	openBodySculptor();
	void	openPartsAssembler();

public:
	void	init() override;
	void	update() override;

	void	makeAsset();

	const shared_ptr<CellAsset>& getCellAsset() const { return m_cellAsset; }
};

