#include "CellMakingViewer.h"
#include "CellAsset.h"
#include "PartAsset.h"
#include "PartConfig.h"
#include "PartAsset_Body.h"

void CellMakingViewer::PartsAssembler::Workspace::init()
{
	setBackgroundColor(Palette::Black);

	setViewerRectInLocal(200, 0, 800, 800);
	setSize(Vec2(800, 800));
}

void CellMakingViewer::PartsAssembler::Workspace::update()
{
	GridViewer::update();

	const auto t1 = getCamera().createTransformer();

	drawParts();

	if (m_selectedPartConfig)
	{
		auto t = Transformer2D(Mat3x2::Translate(m_selectedPartConfig->getPosition()), true);

		if (m_state == State::MoveMode)
		{
			m_selectedPartConfig->setPosition(m_selectedPartConfig->getPosition() + Cursor::DeltaF());

			if (MouseL.down()) m_state = State::RotateMode;
		}

		if (m_state == State::RotateMode)
		{
			if (!Cursor::PreviousPosF().isZero() && !Cursor::PosF().isZero())
			{
				const auto delta = Cursor::PreviousPosF().getAngle(Cursor::PosF());
				
				m_selectedPartConfig->setRotation(m_selectedPartConfig->getRotation() + delta);
			}

			if (MouseL.up()) 
			{
				m_selectedPartConfig = nullptr;
				m_state = State::MoveMode;
			}
		}
	}
	else if (MouseL.down() && isMouseover())
	{
		for (const auto& pc : m_cellAsset->getPartConfigs())
		{
			auto t = Transformer2D(pc->getMat3x2(), true);

			if (pc->getPartAsset()->getShape().getPolygon().mouseOver()) m_selectedPartConfig = pc;
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

void CellMakingViewer::PartsAssembler::Workspace::drawParts() const
{
	for (const auto& p : m_cellAsset->getPartConfigs())
	{
		auto t2 = Transformer2D(p->getMat3x2());

		p->getPartAsset()->getShape().draw(0.5);
		p->getPartAsset()->getShape().getPolygon().drawFrame(1.0, Palette::Black);
	}

	if (m_selectedPartConfig)
	{
		auto t2 = Transformer2D(m_selectedPartConfig->getMat3x2());

		m_selectedPartConfig->getPartAsset()->getShape().getPolygon().draw(ColorF(1.0, 0.5));
	}
}
