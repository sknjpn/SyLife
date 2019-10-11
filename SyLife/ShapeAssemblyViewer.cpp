#include "ShapeAssemblyViewer.h"


void ShapeAssemblyViewer::drawParts() const
{
	auto cmv = g_viewerManagerPtr->getViewer<CellMakingViewer>();

	for (const auto& p : cmv->m_cellAsset->getPartConfigs())
	{
		auto t2 = Transformer2D(Mat3x2::Rotate(p->getRotation())
			.translated(p->getPosition().x, p->getPosition().y));

		for (const auto& s : p->getModel()->getShapes())
			s.m_polygon.draw(ColorF(s.m_color, 0.5)).drawFrame(1.0, Palette::Black);
	}
}

void ShapeAssemblyViewer::drawGrid() const
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

void ShapeAssemblyViewer::init()
{
}

void ShapeAssemblyViewer::update()
{
}
