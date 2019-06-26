#include "TitleViewer.h"

void TitleViewer::Init()
{

}

void TitleViewer::Update()
{
	{
		auto& b = m_bubbles.emplace_back();

		b.m_position = s3d::Vec3(s3d::Random(-50.0, 50.0), -50.0, 100+s3d::Random(-50.0, 50.0));
	}

	// bubbles
	for (auto& b : m_bubbles)
	{
		b.m_position.y += 0.5;

		// draw
		{
			s3d::Vec3 camPos(0.0, 0.0, 0.0);
			s3d::Vec3 p = b.m_position - camPos;

			auto x = (asin(p.x / p.z) / (3.14 / 3.0) + 0.5) * s3d::Window::Size().x;
			auto y = (-asin(p.y / p.z) / (3.14 / 3.0) + 0.5) * s3d::Window::Size().y;
			auto r = 1000.0 / p.length();

			s3d::Circle(x, y, r).draw(s3d::Palette::Lightblue).drawFrame(1.0, s3d::Palette::Black);
		}
	}

	m_bubbles.remove_if([](const auto& b) { return b.m_position.y > 50.0; });
}
