#include "ShapeAssemblyViewer.h"

#include "ViewerManager.h"

#include "ShapeLayerViewer.h"
#include "PartAsset.h"

void ShapeAssemblyViewer::init()
{
	setPriority(2);

	setDrawRect(RectF(800, 800).setCenter(getDrawCenter().movedBy(0, -50)));
	setSize(Vec2(800, 800));
}

void ShapeAssemblyViewer::update()
{
	if (!m_partAsset || isInvisible()) return;

	auto slv = g_viewerManagerPtr->getViewer<ShapeLayerViewer>();
	auto index = slv->getSelectedIndex();

	m_camera.update();

	const auto t1 = m_camera.createTransformer();

	drawGrid();

	// Part
	for (const auto& s : m_partAsset->getShapes())
		s.m_polygon.draw(ColorF(s.m_color, 0.5)).drawFrame(1.0, Palette::Black);

	if (KeyShift.pressed())
		m_circleRadius = Clamp<double>(m_circleRadius * (1.0 + Mouse::Wheel() * 0.1), 1.0, 100.0);

	// Mouse
	Circle(Cursor::PosF(), m_circleRadius).draw(Palette::White);

	if (isMouseOver())
	{
		if (MouseL.pressed())
		{
			m_partAsset->m_shapes[index].m_polygon.append(Circle(Cursor::PosF(), m_circleRadius).asPolygon());
		}

		if (MouseR.pressed())
		{
			auto polygons = Geometry2D::Subtract(m_partAsset->m_shapes[index].m_polygon, Circle(Cursor::PosF(), m_circleRadius).asPolygon());

			if (!polygons.empty()) m_partAsset->m_shapes[index].m_polygon = polygons.front();
		}

		double k = 0.01;
		m_partAsset->m_mass = m_partAsset->m_shapes[0].m_polygon.area() * k;
		m_partAsset->m_material.setNutrition(m_partAsset->m_mass);
	}
}

void ShapeAssemblyViewer::setSize(const Vec2& size)
{
	m_size = size;

	setDrawSize(size);

	m_camera.setScreen(RectF(m_size.x, m_size.y));
	m_camera.setRestrictedRect(RectF(m_size.x, m_size.y).setCenter(Vec2::Zero()));
	m_camera.setCenter(Vec2::Zero());
	m_camera.setTargetCenter(Vec2::Zero());
	m_camera.setMaxScale(4.0);
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
