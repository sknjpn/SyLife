#include "TitleViewer.h"

void TitleViewer::UpdateBubbles()
{
	if (s3d::RandomBool(0.1))
	{
		auto& b = m_bubbles.emplace_back();

		b.m_position = s3d::Vec3(120.0 * s3d::Random(-1.0, 1.0), -100.0, 200.0 + 120.0*s3d::Random(-1.0, 1.0));
	}

	for (auto& b : m_bubbles)
	{
		b.m_timer += 1.0;
		b.m_position.x += 0.05 * s3d::Random(-1.0, 1.0);
		b.m_position.y += 0.15 * s3d::Random(0.25, 1.0);
		b.m_position.z += 0.05 * s3d::Random(-1.0, 1.0);
	}

	m_bubbles.remove_if([](const auto& b) { return b.m_timer > 1800.0; });
}

void TitleViewer::DrawBubbles()
{
	for (auto& b : m_bubbles)
	{
		static double t = 0.0;
		t += 1.0;
		auto s = 5.0;
		s3d::Vec3 camPos(sin(t * 0.00001 * 11) * s, sin(t * 0.00001 * 13) * s, sin(t * 0.00001 * 17) * s);
		s3d::Vec3 p = b.m_position - camPos;

		auto x = (asin(p.x / p.z) / (3.14 / 3.0) + 0.5) * s3d::Window::Size().x;
		auto y = (-asin(p.y / p.z) / (3.14 / 3.0) + 0.5) * s3d::Window::Size().y;
		auto r = 2000.0 / p.length() * s3d::Min(b.m_timer / 1000.0, 1.0);
		auto a = s3d::Min((1800.0 - b.m_timer) / 500.0, 1.0) * 0.1;

		s3d::Circle(x + r * 0.15, y - r * 0.55, r * 0.3).draw(s3d::ColorF(s3d::Palette::Lightblue, a));
		s3d::Circle(x + r * 0.55, y - r * 0.15, r * 0.3).draw(s3d::ColorF(s3d::Palette::Lightblue, a));
		s3d::Circle(x + r * 0.40, y - r * 0.40, r * 0.3).draw(s3d::ColorF(s3d::Palette::Lightblue, a));

		s3d::Circle(x, y, r * 0.3).draw(s3d::ColorF(s3d::Palette::Lightblue, a));
		s3d::Circle(x, y, r * 0.6).draw(s3d::ColorF(s3d::Palette::Lightblue, a));
		s3d::Circle(x, y, r * 1.0).draw(s3d::ColorF(s3d::Palette::Lightblue, a));
	}
}

void TitleViewer::Init()
{
	for (int i = 0; i < 2000; ++i) UpdateBubbles();
}

void TitleViewer::Update()
{
	// bubbles
	{
		for (int i = 0; i < 3; ++i) UpdateBubbles();

		DrawBubbles();
	}

	// title
	{
		static s3d::Font titleFont(256, s3d::Typeface::Bold);
		static auto t = 1.0;
		t += 1.0 / 60.0;

		const auto a = s3d::Min(0.25, t * 0.01);
		const auto p = s3d::Vec2(s3d::Window::Center()).movedBy(0.0, -s3d::Window::Height() * 0.3);
		const auto x = sin(t / 10.0 * 13) * 2.0;
		const auto y = sin(t / 10.0 * 11) * 2.0;

		titleFont(U"SyLife").drawAt(p.movedBy(x * 3.0, y * 3.0), s3d::ColorF(1.0, a));
		titleFont(U"SyLife").drawAt(p.movedBy(x * 2.0, y * 2.0), s3d::ColorF(1.0, a));
		titleFont(U"SyLife").drawAt(p.movedBy(x * 1.0, y * 1.0), s3d::ColorF(1.0, a));
		titleFont(U"SyLife").drawAt(p.movedBy(x * 0.0, y * 0.0), s3d::ColorF(1.0, a));
	}

	// message
	{
		static s3d::Font messageFont(24, s3d::Typeface::Bold);
		static auto t = -5.0;
		t += 1.0 / 60.0;

		const auto a = s3d::Clamp(t * 0.1, 0.0, 0.4);
		const auto p = s3d::Vec2(s3d::Window::Center()).movedBy(0.0, s3d::Window::Height() * 0.3);

		messageFont(U"始めるにはスペースキーを押してください...").drawAt(p, s3d::ColorF(1.0, a * (0.5 + 0.5 * abs(sin(t)))));
	}
}
