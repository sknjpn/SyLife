#include "AssemblyViewer.h"

#include "PartPaletteViewer.h"
#include "CellAsset.h"
#include "PartAsset.h"
#include "PartConfig.h"
#include "BodyAsset.h"

AssemblyViewer::AssemblyViewer()
{
	setViewerRect(RectF(800, 800).setCenter(getDrawCenter().movedBy(0, -50)));
	setSize(Vec2(800, 800));
}

void AssemblyViewer::update()
{
	m_camera.update();

	const auto t1 = m_camera.createTransformer();

	drawGrid();

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
			auto delta = Cursor::PreviousPosF().getAngle(Cursor::PosF());
			m_selectedPartConfig->setRotation(m_selectedPartConfig->getRotation() + delta);

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
			auto t = Transformer2D(Mat3x2::Rotate(pc->getRotation()).translated(pc->getPosition()), true);

			if (pc->getPartAsset()->getShape().getPolygon().mouseOver()) m_selectedPartConfig = pc;
		}
	}

	// selectedPart
	if (auto& selectedPart = getParentViewer()->getChildViewer<PartPaletteViewer>()->getSelectedPart())
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

		if (!MouseL.pressed()) getParentViewer()->getChildViewer<PartPaletteViewer>()->clearSelectedPart();
	}

	m_cellAsset->updateProperties();
}

void AssemblyViewer::setSize(const Vec2& size)
{
	m_size = size;

	setViewerSize(size);

	m_camera.setScreen(RectF(m_size.x, m_size.y));
	m_camera.setRestrictedRect(RectF(m_size.x, m_size.y).setCenter(Vec2::Zero()));
	m_camera.setCenter(Vec2::Zero());
	m_camera.setTargetCenter(Vec2::Zero());
	m_camera.setMaxScale(4.0);
}

void AssemblyViewer::drawParts() const
{
	for (const auto& p : m_cellAsset->getPartConfigs())
	{
		auto t2 = Transformer2D(Mat3x2::Rotate(p->getRotation())
			.translated(p->getPosition().x, p->getPosition().y));

		p->getPartAsset()->getShape().draw(0.5);
		p->getPartAsset()->getShape().getPolygon().drawFrame(1.0, Palette::Black);
	}

	if (m_selectedPartConfig)
	{
		auto t2 = Transformer2D(Mat3x2::Rotate(m_selectedPartConfig->getRotation())
			.translated(m_selectedPartConfig->getPosition()));

		m_selectedPartConfig->getPartAsset()->getShape().getPolygon().draw(ColorF(1.0, 0.5));
	}
}

void AssemblyViewer::drawGrid() const
{
	const int scale = (int)log10(m_camera.getScale());
	const double thickness = 2.0 / m_camera.getScale();
	const double interval = pow(10.0, -scale + 1);

	// 縦線
	{
		const auto color = ColorF(Palette::White, 0.25);

		for (double x = -m_size.x / 2.0; x <= m_size.x / 2.0; x += interval)
			Line(x, -m_size.y / 2.0, x, m_size.y / 2.0).draw(thickness, color);

		for (double y = -m_size.y / 2.0; y <= m_size.y / 2.0; y += interval)
			Line(-m_size.x / 2.0, y, m_size.x / 2.0, y).draw(thickness, color);
	}

	// XY軸
	{
		Line(-m_size.x / 2.0, 0, m_size.x / 2.0, 0).draw(thickness, Palette::Red);
		Line(0, -m_size.y / 2.0, 0, m_size.y / 2.0).draw(thickness, Palette::Red);
	}
}
