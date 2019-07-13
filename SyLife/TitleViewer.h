#pragma once

#include "Viewer.h"

#include "FieldViewer.h"
#include "EditorViewer.h"
#include "Curtain.h"

class TitleViewer
	: public Viewer
{
	class Bubble
	{
	public:
		Vec3	m_position;
		double		m_timer = 0.0;
	};

	Array<Bubble>	m_bubbles;
	Audio m_audio;

public:
	TitleViewer()
		: m_audio(U"assets/music/_”é‚Ì¢ŠE.mp3")
	{
		SetDrawRect(Scene::Size());
		m_audio.setLoop(true);
		m_audio.play();
	}

	void	UpdateBubbles()
	{
		if (RandomBool(0.8))
		{
			auto& b = m_bubbles.emplace_back();

			b.m_position = Vec3(120.0 * Random(-1.0, 1.0), -100.0, 150.0 + 120.0*Random(-1.0, 1.0));
		}
		static PerlinNoise noise1(Random(0xFFFFFFFF));
		static PerlinNoise noise2(Random(0xFFFFFFFF));
		static Vec3 liner(0, 0, 0);
		for (auto& b : m_bubbles)
		{
			b.m_timer += 1.0;
			b.m_position.x += 0.15 * noise1.noise(b.m_position * 0.02 + liner);
			b.m_position.y += 0.15 * Random(0.25, 1.0);
			b.m_position.z += 0.15 * noise2.noise(b.m_position * 0.02 + liner);
		}
		liner.moveBy(0, 0, 0.01);

		m_bubbles.remove_if([](const auto& b) { return b.m_timer > 1800.0; });
	}

	void	DrawBubbles()
	{
		static Texture texture(U"particle.png", TextureDesc::Mipped);
		
		ScopedRenderStates2D blend(BlendState::Additive);

		static double t = 0.0;
		t += 1.0;
		for (auto& b : m_bubbles)
		{
			auto s = 5.0;
			Vec3 camPos(sin(t * 0.001 * 11) * s, sin(t * 0.001 * 13) * s, sin(t * 0.001 * 17) * s);
			Vec3 p = b.m_position - camPos;

			auto x = (asin(p.x / p.z) / (3.14 / 3.0) + 0.5) * Scene::Size().x;
			auto y = (-asin(p.y / p.z) / (3.14 / 3.0) + 0.5) * Scene::Size().y;
			auto r = 2000.0 / p.length() * Min(b.m_timer / 1000.0, 1.0) * 10.0;
			auto a = Min((1800.0 - b.m_timer) / 500.0, 1.0) * 0.1;

			/*texture.resized(r* 0.3).drawAt(x + r * 0.15, y - r * 0.55, ColorF(Palette::Lightblue, a));
			texture.resized(r* 0.3).drawAt(x + r * 0.55, y - r * 0.15, ColorF(Palette::Lightblue, a));
			texture.resized(r* 0.3).drawAt(x + r * 0.40, y - r * 0.40, ColorF(Palette::Lightblue, a));*/
			//texture.resized(r* 0.3).drawAt(x, y, ColorF(Palette::Lightblue, a));
			texture.resized(r* 0.6).drawAt(x, y, ColorF(Palette::Lightblue, a));
			texture.resized(r* 1.0).drawAt(x, y, ColorF(Palette::Lightblue, a));
			/*
						Circle(x + r * 0.15, y - r * 0.55, r * 0.3).draw(ColorF(Palette::Lightblue, a));
						Circle(x + r * 0.55, y - r * 0.15, r * 0.3).draw(ColorF(Palette::Lightblue, a));
						Circle(x + r * 0.40, y - r * 0.40, r * 0.3).draw(ColorF(Palette::Lightblue, a));

						Circle(x, y, r * 0.3).draw(ColorF(Palette::Lightblue, a));
						Circle(x, y, r * 0.6).draw(ColorF(Palette::Lightblue, a));
						Circle(x, y, r * 1.0).draw(ColorF(Palette::Lightblue, a));
						*/
		}
	}

	void	Init() override
	{
		for (int i = 0; i < 2000; ++i) UpdateBubbles();
	}

	void	Update() override
	{

		// title
		{
			auto te = Transformer2D(Mat3x2::Scale(1.4, Scene::Center()));
			static Font titleFont(256, Typeface::Bold);
			static auto t = 1.0;
			t += 1.0 / 60.0;

			const auto a = 0.25;// Min(0.25, t * 0.01);
			const auto p = Vec2(Scene::Center()).movedBy(0.0, -Scene::Height() * 0.1);
			const auto x = sin(t / 10.0 * 13) * 5.0;
			const auto y = sin(t / 10.0 * 11) * 5.0;

			titleFont(U"SyLife").drawAt(p.movedBy(x * 3.0, y * 3.0), ColorF(1.0, a));
			titleFont(U"SyLife").drawAt(p.movedBy(x * 2.0, y * 2.0), ColorF(1.0, a));
			titleFont(U"SyLife").drawAt(p.movedBy(x * 1.0, y * 1.0), ColorF(1.0, a));
			titleFont(U"SyLife").drawAt(p.movedBy(x * 0.0, y * 0.0), ColorF(1.0, a));
		}

		// bubbles
		{
			for (int i = 0; i < 3; ++i) UpdateBubbles();

			DrawBubbles();
		}

		// message
		{
			static Font messageFont(48, Typeface::Bold);
			static auto t = -5.0;
			t += 1.0 / 60.0;

			const auto a = 0.4;// Clamp(t * 0.1, 0.0, 0.4);

			{
				auto f1 = messageFont(U"SyLife");
				auto r1 = f1.region().setCenter(0, 0);

				{
					auto t2 = Transformer2D(Mat3x2::Translate(Vec2(Scene::Center()).movedBy(0.0, Scene::Height() * 0.3)).translated(0, -48), true);
					r1.draw(r1.mouseOver() ? ColorF(1.0, 0.5) : ColorF(0.0, 0.0));
					f1.drawAt(Vec2::Zero(), ColorF(1.0, a * (0.5 + 0.5 * abs(sin(t)))));
				}

				// scene‘JˆÚ
				{
					auto t2 = Transformer2D(Mat3x2::Identity(), Mat3x2::Translate(Vec2(Scene::Center()).movedBy(0.0, Scene::Height() * 0.3)).translated(0, -48));

					static bool f = false;

					if (r1.leftClicked()) f = true;

					if (f)
					{
						static Curtain closeCurtain(Color(11, 22, 33), 1.0);
						closeCurtain.CloseUpdate();
						m_audio.setVolume(Max(1.0 - closeCurtain.m_st.sF() / closeCurtain.m_duration, 0.0));

						if (closeCurtain.m_st.sF() > 1.0)
						{
							g_viewerManagerPtr->ClearViewers();
							g_viewerManagerPtr->MakeViewer<FieldViewer>();

							return;
						}
					}
				}
			}

			{
				auto f2 = messageFont(U"Editor");
				auto r2 = f2.region().setCenter(0, 0);

				{
					auto t2 = Transformer2D(Mat3x2::Translate(Vec2(Scene::Center()).movedBy(0.0, Scene::Height() * 0.3)).translated(0, 48), true);
					r2.draw(r2.mouseOver() ? ColorF(1.0, 0.5) : ColorF(0.0, 0.0));
					f2.drawAt(Vec2::Zero(), ColorF(1.0, a * (0.5 + 0.5 * abs(sin(t)))));
				}

				// scene‘JˆÚ
				{
					auto t2 = Transformer2D(Mat3x2::Identity(), Mat3x2::Translate(Vec2(Scene::Center()).movedBy(0.0, Scene::Height() * 0.3)).translated(0, 48));

					static bool f = false;

					if (r2.leftClicked()) f = true;

					if (f)
					{
						static Curtain closeCurtain(Color(11, 22, 33), 1.0);
						closeCurtain.CloseUpdate();
						m_audio.setVolume(Max(1.0 - closeCurtain.m_st.sF() / closeCurtain.m_duration, 0.0));

						if (closeCurtain.m_st.sF() > 1.0)
						{
							g_viewerManagerPtr->ClearViewers();
							g_viewerManagerPtr->MakeViewer<EditorViewer>();

							return;
						}
					}
				}
			}
		}

		static Curtain curtain(Color(11, 22, 33), 1.0);
		curtain.OpenUpdate();
		m_audio.setVolume(Min(curtain.m_st.sF() / curtain.m_duration, 1.0));
	}
};

