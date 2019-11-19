#include "GridViewer.h"

void GridViewer::init()
{
	setBackgroundColor(Palette::Black);

	setSize(Vec2(800, 800));
}

void GridViewer::update()
{
	m_camera.update();

	{
		const auto t1 = m_camera.createTransformer();

		drawGrid();
	}
}

void GridViewer::setSize(const Vec2& size)
{
	setViewerSize(size);

	m_camera.setScreen(RectF(size.x, size.y));
	m_camera.setRestrictedRect(RectF(size.x, size.y).setCenter(Vec2::Zero()));
	m_camera.setCenter(Vec2::Zero());
	m_camera.setTargetCenter(Vec2::Zero());
	m_camera.setMaxScale(4.0);
}

void GridViewer::drawGrid() const
{
	const int scale = (int)log10(m_camera.getScale());
	const double thickness = 2.0 / m_camera.getScale();
	const double interval = pow(10.0, -scale + 1);
	const auto size = getViewerSize();

	// 縦線
	{
		const auto color = ColorF(Palette::White, 0.25);

		for (double x = -size.x / 2.0; x <= size.x / 2.0; x += interval)
			Line(x, -size.y / 2.0, x, size.y / 2.0).draw(thickness, color);

		for (double y = -size.y / 2.0; y <= size.y / 2.0; y += interval)
			Line(-size.x / 2.0, y, size.x / 2.0, y).draw(thickness, color);
	}

	// XY軸
	{
		Line(-size.x / 2.0, 0, size.x / 2.0, 0).draw(thickness, Palette::Red);
		Line(0, -size.y / 2.0, 0, size.y / 2.0).draw(thickness, Palette::Red);
	}
}
