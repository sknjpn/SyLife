#include "AssemblyViewer.h"

#include "ViewerManager.h"

#include "CellMakingViewer.h"
#include "PartPaletteViewer.h"
#include "CellAsset.h"
#include "PartAsset.h"
#include "PartConfig.h"
#include "BodyAsset.h"

const shared_ptr<CellAsset>& AssemblyViewer::getCellAsset() const
{
	return g_viewerManagerPtr->getViewer<CellMakingViewer>()->m_cellAsset;
}

const shared_ptr<BodyAsset>& AssemblyViewer::getBodyAsset() const
{
	return g_viewerManagerPtr->getViewer<CellMakingViewer>()->m_bodyAsset;
}

void AssemblyViewer::init()
{
	setPriority(2);

	setDrawRect(RectF(800, 800).setCenter(getDrawCenter().movedBy(0, -50)));
	setSize(Vec2(800, 800));
}

void AssemblyViewer::update()
{
	auto cmv = g_viewerManagerPtr->getViewer<CellMakingViewer>();

	m_camera.update();

	const auto t1 = m_camera.createTransformer();

	drawGrid();

	drawParts();

	if (MouseL.down()) m_grabCircle = false;
	if (MouseL.down() && m_selectedPartConfig)
	{
		auto t = Transformer2D(Mat3x2::Translate(m_selectedPartConfig->getPosition()), true);
		auto r = m_selectedPartConfig->getModel()->getShape().getRadius();

		if (Circle(Vec2::Down() * r, r * 0.2).mouseOver())
		{
			m_grabCircle = true;
		}
		else m_selectedPartConfig = nullptr;
	}
	if (MouseL.down() && isMouseOver())
	{
		for (const auto& pc : getCellAsset()->getPartConfigs())
		{
			auto t2 = Transformer2D(Mat3x2::Rotate(pc->getRotation())
				.translated(pc->getPosition()), true);

			if (pc->getModel()->getShape().getPolygon().mouseOver()) m_selectedPartConfig = pc;
		}
	}

	if (m_selectedPartConfig)
	{
		auto t = Transformer2D(Mat3x2::Translate(m_selectedPartConfig->getPosition()), true);
		auto r = m_selectedPartConfig->getModel()->getShape().getRadius();

		Geometry2D::Subtract(Circle(Vec2::Down() * r * 0.1, r).asPolygon(), Circle(Vec2::Up() * r * 0.1, r).asPolygon()).front().draw();

		if (m_grabCircle)
		{
			auto delta = Cursor::PreviousPosF().getAngle(Cursor::PosF());
			m_selectedPartConfig->setRotation(m_selectedPartConfig->getRotation() + delta);

			Circle(Cursor::PosF().setLength(r), r * 0.2).draw(Palette::Orange);
		}
		else
		{
			Circle(Vec2::Down() * r, r * 0.2).draw(Palette::Orange);
		}
	}

	// selectedPart
	if (auto& selectedPart = g_viewerManagerPtr->getViewer<PartPaletteViewer>()->getSelectedPart())
	{
		bool canSetPart = getBodyAsset()->getShape().getPolygon().contains(Cursor::PosF());

		if (isMouseOver())
		{
			{
				auto t = Transformer2D(Mat3x2::Translate(Cursor::PosF()));

				selectedPart->getShape().draw(0.5);
			}

			if (MouseL.up() && canSetPart)
			{
				const auto& partConfig = cmv->m_cellAsset->addPartConfig();

				partConfig->setModel(selectedPart);
				partConfig->setPosition(Vec2(Cursor::PosF().x, Cursor::PosF().y));
				partConfig->setRotation(0.0);
			}

			selectedPart->getShape().getPolygon().movedBy(Cursor::PosF()).draw(ColorF(canSetPart ? Palette::Green : Palette::Red, 0.5));
		}

		if (!MouseL.pressed()) g_viewerManagerPtr->getViewer<PartPaletteViewer>()->clearSelectedPart();
	}

	cmv->m_cellAsset->updateProperties();
}

void AssemblyViewer::setSize(const Vec2& size)
{
	m_size = size;

	setDrawSize(size);

	m_camera.setScreen(RectF(m_size.x, m_size.y));
	m_camera.setRestrictedRect(RectF(m_size.x, m_size.y).setCenter(Vec2::Zero()));
	m_camera.setCenter(Vec2::Zero());
	m_camera.setTargetCenter(Vec2::Zero());
	m_camera.setMaxScale(4.0);
}

void AssemblyViewer::drawParts() const
{
	auto cmv = g_viewerManagerPtr->getViewer<CellMakingViewer>();

	for (const auto& p : cmv->m_cellAsset->getPartConfigs())
	{
		auto t2 = Transformer2D(Mat3x2::Rotate(p->getRotation())
			.translated(p->getPosition().x, p->getPosition().y));

		p->getModel()->getShape().draw(0.5);
		p->getModel()->getShape().getPolygon().drawFrame(1.0, Palette::Black);
	}

	if (m_selectedPartConfig)
	{
		auto t2 = Transformer2D(Mat3x2::Rotate(m_selectedPartConfig->getRotation())
			.translated(m_selectedPartConfig->getPosition()));

		m_selectedPartConfig->getModel()->getShape().getPolygon().draw(ColorF(1.0, 0.5));
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
