#include "CellMakingViewer.h"
#include "PartAsset.h"

void CellMakingViewer::BodySculptor::Workspace::init()
{
	setBackgroundColor(Palette::Gray);

	setViewerRect(RectF(800, 800).setCenter(getDrawCenter().movedBy(0, -50)));
	setSize(Vec2(800, 800));
}

void CellMakingViewer::BodySculptor::Workspace::update()
{
	if (!m_partAsset) return;

	auto slv = getParentViewer()->getChildViewer<LayerLists>();
	auto index = slv->getSelectedIndex();

	m_camera.update();

	const auto t1 = m_camera.createTransformer();

	drawGrid();

	m_partAsset->m_shape.updateProperties();

	// Part
	m_partAsset->getShape().draw(0.5);
	m_partAsset->getShape().getPolygon().drawFrame(1.0, Palette::Black);

	if (KeyShift.pressed())
		m_circleRadius = Clamp<double>(m_circleRadius * (1.0 + Mouse::Wheel() * 0.1), 1.0, 100.0);

	// Mouse
	Circle(Cursor::PosF(), m_circleRadius).draw(ColorF(m_partAsset->m_shape[index].m_color, 0.5));

	if (isMouseover())
	{
		if (MouseL.pressed())
		{
			m_partAsset->m_shape[index].m_polygon.append(Circle(Cursor::PosF(), m_circleRadius).asPolygon());
		}

		if (MouseR.pressed())
		{
			const auto polygons = Geometry2D::Subtract(m_partAsset->m_shape[index].m_polygon, Circle(Cursor::PosF(), m_circleRadius).asPolygon());

			if (polygons.empty()) m_partAsset->m_shape[index].m_polygon = Polygon();
			else
			{
				int maxIndex = 0;
				for (int i = 1; i < polygons.size(); ++i)
					if (polygons[maxIndex].area() < polygons[i].area()) maxIndex = i;

				m_partAsset->m_shape[index].m_polygon = polygons[maxIndex];
			}
		}

		double k = 0.01;
		m_partAsset->m_mass = m_partAsset->m_shape[0].m_polygon.area() * k;
		m_partAsset->m_material.setNutrition(m_partAsset->m_mass);
	}
}

void CellMakingViewer::BodySculptor::Workspace::setSize(const Vec2& size)
{
	m_size = size;

	setViewerSize(size);

	m_camera.setScreen(RectF(m_size.x, m_size.y));
	m_camera.setRestrictedRect(RectF(m_size.x, m_size.y).setCenter(Vec2::Zero()));
	m_camera.setCenter(Vec2::Zero());
	m_camera.setTargetCenter(Vec2::Zero());
	m_camera.setMaxScale(4.0);
}

void CellMakingViewer::BodySculptor::Workspace::drawGrid() const
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
