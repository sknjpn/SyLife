#include "ReleaseViewer.h"

void ReleaseViewer::Update(bool isMouseOver)
{
	s3d::Rect(m_drawRect.size.asPoint()).draw(s3d::Color(11, 22, 33, 192));

	s3d::Circle circle(50, 50, 45);

	circle
		.draw(circle.mouseOver() ? s3d::Palette::Orange : s3d::Palette::Skyblue)
		.drawFrame(4.0, s3d::Palette::Black);

	if (m_isDragged) circle.draw(s3d::Palette::Red);

	if (circle.leftClicked()) m_isDragged = true;
	if (!s3d::MouseL.pressed()) m_isDragged = false;

	// part
	{
		auto t1 = s3d::Transformer2D(s3d::Mat3x2::Scale(45.0 / m_model->m_radius / 2.0).translated(circle.center));

		for (const auto& p : m_model->m_partConfigs)
		{
			auto t2 = s3d::Transformer2D(s3d::Mat3x2::Rotate(p->m_rotation).translated(p->m_position.m_x, p->m_position.m_y));

			for (const auto& s : p->m_model->GetShapes())
				s.m_polygon.draw(s3d::ColorF(s.m_color, 0.5)).drawFrame(1.0, s3d::Palette::Black);
		}
	}
}