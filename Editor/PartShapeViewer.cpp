#include "PartShapeModelViewer.h"
#include "AssetManager.h"
#include "BodyModel.h"

void PartShapeModelViewer::Init()
{
	m_camera.setCenter(m_drawRect.center());
}

void PartShapeModelViewer::Update()
{
	if (IsMouseOver()) m_camera.update();

	const auto t = m_camera.createTransformer();
	const int scale = log10(m_camera.getMagnification());
	const double thickness = 2.0 / m_camera.getMagnification();
	const double interval = pow(10.0, -scale + 1);
	const auto cursor = (s3d::Cursor::Pos() / interval).asPoint() * interval;
	const auto boxSize = s3d::Vec2::One() * 5.0 / m_camera.getMagnification();
	const auto pointer = s3d::RectF(s3d::Arg::center(cursor), boxSize);

	// 縦線
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

	// XY軸
	{
		s3d::Line(m_camera.getCameraRect().x, 0, m_camera.getCameraRect().br().x, 0).draw(thickness, s3d::Palette::Red);
		s3d::Line(0, m_camera.getCameraRect().y, 0, m_camera.getCameraRect().br().y).draw(thickness, s3d::Palette::Red);
	}

	// ShapeModels
	for (const auto& s : m_model->m_shapes)
	{
		// Face
		{
			s3d::Polygon p(s.m_verticles);

			p.draw(s3d::ColorF(s.m_color, 0.5));
		}

		// Line
		for (auto it = s.m_verticles.begin(); it != s.m_verticles.end(); ++it)
		{
			s3d::Line l = (it == s.m_verticles.end() - 1) ? s3d::Line(s.m_verticles.front(), s.m_verticles.back()) : s3d::Line(*it, *(it + 1));

			l.draw(thickness, s3d::ColorF(s.m_color, 0.5));
		}

		// Verticle
		for (auto it = m_verticles.begin(); it != m_verticles.end(); ++it)
		{
			auto r = s3d::RectF(s3d::Arg::center(*it), boxSize);

			r.draw(r.mouseOver() ? s3d::Palette::Blue : s.m_color);
		}
	}

	// Verticles
	{
		// Pointer
		pointer.draw(s3d::Palette::Red);

		if (!m_verticles.empty())
		{
			// Line
			for (auto it = m_verticles.begin(); it < m_verticles.end() - 1; ++it)
				s3d::Line(*it, *(it + 1)).draw(thickness, s3d::ColorF(s3d::Palette::White, 0.5));

			// Last Line
			s3d::Line(m_verticles.back(), cursor).draw(thickness, s3d::ColorF(s3d::Palette::Gray, 0.5));

			// Verticle
			for (auto it = m_verticles.begin(); it != m_verticles.end(); ++it)
			{
				auto r = s3d::RectF(s3d::Arg::center(*it), boxSize);

				r.draw(*it == cursor ? s3d::Palette::Blue : s3d::Palette::White);
			}
		}
	}

	// Update
	if (IsMouseOver())
	{
		// Verticleの配置
		if (m_verticles.empty() && s3d::MouseL.down()) m_verticles.emplace_back(cursor);
		else if (!m_verticles.empty())
		{
			// Verticleを繋げて固定
			for (auto it = m_verticles.begin(); it != m_verticles.end(); ++it)
			{
				if (*it == cursor && s3d::MouseL.down())
				{
					// Connect
					if (it != m_verticles.begin()) m_verticles.erase(m_verticles.begin(), it);

					m_model->m_shapes.emplace_back(m_verticles);

					m_verticles.clear();

					return;
				}
			}

			// 連続配置
			if (s3d::MouseL.down()) m_verticles.emplace_back(cursor);

			// 最後のVerticleを削除
			if (s3d::MouseR.down()) m_verticles.pop_back();
		}
	}

	// Save
	if (s3d::KeyControl.pressed() && s3d::KeyS.down())
	{
		if (m_model->m_name.empty()) m_model->m_name = "None";
		if (m_model->m_mass <= 0) m_model->m_mass = 10;

		m_model->Save();
	}
}
