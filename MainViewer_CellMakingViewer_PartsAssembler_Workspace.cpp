#include "MainViewer.h"
#include "CellAsset.h"
#include "PartAsset.h"
#include "PartConfig.h"
#include "PartAsset_Body.h"
#include "GUIButton.h"

void MainViewer::CellMakingViewer::PartsAssembler::Workspace::init()
{
	setBackgroundColor(Palette::Black);

	setViewerRectInLocal(200, 0, 800, 800);
}

void MainViewer::CellMakingViewer::PartsAssembler::Workspace::update()
{
	auto t = Transformer2D(Mat3x2::Scale(4).translated(400, 400), true);

	drawParts();

	if (MouseL.up() && m_selectedPartConfig)
	{
		if (m_state == State::MoveMode)
		{
			bool canSetPart = m_cellAsset->getBodyAsset()->getShape().getPolygon().contains(m_selectedPartConfig->getPosition());

			if (!canSetPart)
			{
				m_selectedPartConfig->setPosition(m_prePosition);
			}
		}

		m_selectedPartConfig = nullptr;
	}

	if (m_selectedPartConfig)
	{
		if (m_state == State::MoveMode)
		{
			Vec2 position = m_selectedPartConfig->getPosition() + Cursor::DeltaF();
			m_selectedPartConfig->setPosition(position);

			if (MouseL.down()) m_state = State::RotateMode;

			bool canSetPart = m_cellAsset->getBodyAsset()->getShape().getPolygon().contains(position);
			m_selectedPartConfig->getPartAsset()->getShape().getPolygon().movedBy(position).draw(ColorF(canSetPart ? Palette::Green : Palette::Red, 0.5));
		}

		if (m_state == State::RotateMode)
		{
			auto t = Transformer2D(Mat3x2::Translate(m_selectedPartConfig->getPosition()), true);

			if (!Cursor::PreviousPosF().isZero() && !Cursor::PosF().isZero())
			{
				const auto delta = Cursor::PreviousPosF().getAngle(Cursor::PosF());

				m_selectedPartConfig->setRotation(m_selectedPartConfig->getRotation() + delta);
			}
		}
	}
	else if (MouseL.down() && isMouseover())
	{
		for (const auto& pc : m_cellAsset->getPartConfigs())
		{
			if (dynamic_pointer_cast<PartAsset_Body>(pc->getPartAsset())) continue;

			auto t = Transformer2D(pc->getMat3x2(), true);

			if (pc->getPartAsset()->getShape().getPolygon().mouseOver())
			{
				m_selectedPartConfig = pc;
				m_prePosition = m_selectedPartConfig->getPosition();
				m_preRotation = m_selectedPartConfig->getRotation();
			}
		}
	}

	// selectedPart
	if (auto& selectedPart = getParentViewer()->getChildViewer<PartList>()->getSelectedPart())
	{
		bool canSetPart = m_cellAsset->getBodyAsset()->getShape().getPolygon().contains(Cursor::PosF());

		if (isMouseover())
		{
			{
				auto t = Transformer2D(Mat3x2::Translate(Cursor::PosF()));

				selectedPart->getShape().draw(0.5);
			}

			if (MouseL.up() && canSetPart)
			{
				const auto& partConfig = m_cellAsset->addPartConfig();

				partConfig->setPartAsset(selectedPart);
				partConfig->setPosition(Vec2(Cursor::PosF().x, Cursor::PosF().y));
				partConfig->setRotation(0.0);
			}

			selectedPart->getShape().getPolygon().movedBy(Cursor::PosF()).draw(ColorF(canSetPart ? Palette::Green : Palette::Red, 0.5));
		}

		if (!MouseL.pressed()) getParentViewer()->getChildViewer<PartList>()->clearSelectedPart();
	}
}

void MainViewer::CellMakingViewer::PartsAssembler::Workspace::drawParts() const
{
	for (const auto& partConfig : m_cellAsset->getPartConfigs())
	{
		{
			auto t2 = Transformer2D(partConfig->getMat3x2());

			partConfig->getPartAsset()->getShape().draw(0.5);
			partConfig->getPartAsset()->getShape().getPolygon().drawFrame(1.0, Palette::Black);
		}

		if (m_state == State::RotateMode && !dynamic_pointer_cast<PartAsset_Body>(partConfig->getPartAsset())) Circle(partConfig->getPosition(), 2.0).draw(Palette::Yellow).drawFrame(1.0, Palette::Black);
	}

	if (m_selectedPartConfig)
	{
		auto t2 = Transformer2D(m_selectedPartConfig->getMat3x2());

		m_selectedPartConfig->getPartAsset()->getShape().getPolygon().draw(ColorF(1.0, 0.5));
	}
}

void	MainViewer::CellMakingViewer::PartsAssembler::Workspace::setMoveMode()
{
	m_state = State::MoveMode;
	getParentViewer()->getChildViewer<GUIButton>(U"移動モード")->setIsEnabled(false);
	getParentViewer()->getChildViewer<GUIButton>(U"回転モード")->setIsEnabled(true);
}

void	MainViewer::CellMakingViewer::PartsAssembler::Workspace::setRotateMode()
{
	m_state = State::RotateMode;
	getParentViewer()->getChildViewer<GUIButton>(U"移動モード")->setIsEnabled(true);
	getParentViewer()->getChildViewer<GUIButton>(U"回転モード")->setIsEnabled(false);
}
