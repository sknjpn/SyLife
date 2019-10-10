#include "PartShapeViewer.h"
#include "BodyAsset.h"

void PartShapeViewer::init()
{
}

void PartShapeViewer::update()
{
	m_camera.update();

	const auto t = m_camera.createTransformer();
	const int scale = (int)log10(m_camera.getScale());
	const double thickness = 2.0 / m_camera.getScale();
	const double interval = pow(10.0, -scale + 1);
	const auto cursor = (Cursor::Pos() / interval).asPoint() * interval;
	const auto boxSize = Vec2::One() * 5.0 / m_camera.getScale();
	const auto pointer = RectF(Arg::center(cursor), boxSize);

	// 縦線
	{
		const auto color = ColorF(Palette::White, 0.25);

		for (double x = 0; x >= m_camera.getCameraRect().x; x -= interval)
			Line(x, m_camera.getCameraRect().y, x, m_camera.getCameraRect().br().y).draw(thickness, color);

		for (double x = 0; x <= m_camera.getCameraRect().br().x; x += interval)
			Line(x, m_camera.getCameraRect().y, x, m_camera.getCameraRect().br().y).draw(thickness, color);

		for (double y = 0; y >= m_camera.getCameraRect().y; y -= interval)
			Line(m_camera.getCameraRect().x, y, m_camera.getCameraRect().br().x, y).draw(thickness, color);

		for (double y = 0; y <= m_camera.getCameraRect().br().y; y += interval)
			Line(m_camera.getCameraRect().x, y, m_camera.getCameraRect().br().x, y).draw(thickness, color);
	}

	// XY軸
	{
		Line(m_camera.getCameraRect().x, 0, m_camera.getCameraRect().br().x, 0).draw(thickness, Palette::Red);
		Line(0, m_camera.getCameraRect().y, 0, m_camera.getCameraRect().br().y).draw(thickness, Palette::Red);
	}

	// ShapeModels
	for (const auto& s : getModel<PartAsset>()->getShapes())
	{
		// Face
		s.GetPolygon().draw(ColorF(s.m_color, 0.5));

		// Line
		/*for (auto it = s.m_verticles.begin(); it != s.m_verticles.end(); ++it)
		{
			Line l = (it == s.m_verticles.end() - 1) ? Line(s.m_verticles.front(), s.m_verticles.back()) : Line(*it, *(it + 1));

			l.draw(thickness, ColorF(s.m_color, 0.5));
		}*/

		// Verticle
		for (auto it = m_verticles.begin(); it != m_verticles.end(); ++it)
		{
			auto r = RectF(Arg::center(*it), boxSize);

			r.draw(r.mouseOver() ? Palette::Blue : s.m_color);
		}
	}

	// Verticles
	{
		// Pointer
		pointer.draw(Palette::Red);

		if (!m_verticles.empty())
		{
			// Line
			for (auto it = m_verticles.begin(); it < m_verticles.end() - 1; ++it)
				Line(*it, *(it + 1)).draw(thickness, ColorF(Palette::White, 0.5));

			// Last Line
			Line(m_verticles.back(), cursor).draw(thickness, ColorF(Palette::Gray, 0.5));

			// Verticle
			for (auto it = m_verticles.begin(); it != m_verticles.end(); ++it)
			{
				auto r = RectF(Arg::center(*it), boxSize);

				r.draw(*it == cursor ? Palette::Blue : Palette::White);
			}
		}
	}

	// update
	if (isMouseOver())
	{
		// Verticleの配置
		if (m_verticles.empty() && MouseL.down()) m_verticles.emplace_back(cursor);
		else if (!m_verticles.empty())
		{
			// Verticleを繋げて固定
			for (auto it = m_verticles.begin(); it != m_verticles.end(); ++it)
			{
				if (*it == cursor && MouseL.down())
				{
					// Connect
					if (it != m_verticles.begin()) m_verticles.erase(m_verticles.begin(), it);

					//m_model->getShapes().emplace_back(m_verticles);

					m_verticles.clear();

					return;
				}
			}

			// 連続配置
			if (MouseL.down()) m_verticles.emplace_back(cursor);

			// 最後のVerticleを削除
			if (MouseR.down()) m_verticles.pop_back();
		}
	}
}
