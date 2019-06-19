#include "AssemblyViewer.h"
#include "AssetManager.h"

void AssemblyViewer::Init()
{
	m_camera.setCenter(m_drawRect.center());
}

void AssemblyViewer::Update()
{
	if (IsMouseOver()) m_camera.update();

	const auto t = m_camera.createTransformer();
	const int scale = log10(m_camera.getMagnification());
	const double thickness = 2.0 / m_camera.getMagnification();
	const double interval = pow(10.0, -scale + 1);
	const auto cursor = (s3d::Cursor::Pos() / interval).asPoint() * interval;
	const auto boxSize = s3d::Vec2::One() * 5.0 / m_camera.getMagnification();
	const auto pointer = s3d::RectF(s3d::Arg::center(cursor), boxSize);

	// cü
	{
		const auto color = s3d::ColorF(s3d::Palette::White, 0.25);

		for (double x = 0; x >= m_camera.getCameraRect().x; x -= interval)
			s3d::Line(x, m_camera.getCameraRect().y, x, m_camera.getCameraRect().br().y).draw(thickness, color);

		for (double x = 0; x <= m_camera.getCameraRect().br().x; x += interval)
			s3d::Line(x, m_camera.getCameraRect().y, x, m_camera.getCameraRect().br().y).draw(thickness, color);

		for (double y = 0; y >= m_camera.getCameraRect().y; y -= interval)
			s3d::Line(m_camera.getCameraRect().x, y, m_camera.getCameraRect().br().x, y).draw(thickness, color);

		for (double y = 0; y <= m_camera.getCameraRect().br().y; y += interval)
			s3d::Line(m_camera.getCameraRect().x, y, m_camera.getCameraRect().br().x, y).draw(thickness, color);
	}

	// XYŽ²
	{
		s3d::Line(m_camera.getCameraRect().x, 0, m_camera.getCameraRect().br().x, 0).draw(thickness, s3d::Palette::Red);
		s3d::Line(0, m_camera.getCameraRect().y, 0, m_camera.getCameraRect().br().y).draw(thickness, s3d::Palette::Red);
	}

	// body
	if (m_model->m_body->m_model != nullptr)
	{
		for (const auto& s : m_model->m_body->m_model->m_shapes)
		{
			s3d::Polygon(s.m_verticles).draw(s.m_color);
		}
	}
}
