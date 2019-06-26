#include "TitleViewer.h"

void TitleViewer::Init()
{
	for (int i = 0; i < 100; ++i)
	{
		auto& b = m_bubbles.emplace_back();

		b.m_position = s3d::RandomVec3(100.0).moveBy(0.0, 0.0, 100.0);
	}
}

void TitleViewer::Update()
{
	// bubbles
	for (const auto& b : m_bubbles)
	{
		// draw
		{
			s3d::Vec3 camPos(0.0, 0.0, 0.0);
			s3d::Vec3 p = b.m_position - camPos;

			auto x = asin(p.x / p.z) / (3.14 / 3.0) * s3d::Window::Size().x;
			auto y = asin(p.y / p.z) / (3.14 / 3.0) * s3d::Window::Size().y;
			auto r = 10.0 / p.length();

			s3d::Circle(x, y, r).draw(s3d::Palette::Lightblue).drawFrame(1.0, s3d::Palette::Black);
		}
	}
}
