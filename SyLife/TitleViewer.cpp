#include "TitleViewer.h"
#include "FieldViewer.h"
#include "EditorViewer.h"
#include "GUIButton.h"

TitleViewer::TitleViewer()
	: m_audio(U"assets/music/神秘の世界.mp3")
	, m_closeCurtain(Color(0, 0), Color(11, 22, 33), 0.5)
	, m_openCurtain(Color(11, 22, 33), Color(0, 0), 0.5, true)
{
	setViewerRect(Scene::Size());
	m_audio.setLoop(true);
	//m_audio.play();

	for (int i = 0; i < 2000; ++i) UpdateBubbles();
}

void TitleViewer::UpdateBubbles()
{
	if (RandomBool(0.8))
	{
		auto& b = m_bubbles.emplace_back();

		b.m_position = Vec3(120.0 * Random(-1.0, 1.0), -100.0, 150.0 + 120.0 * Random(-1.0, 1.0));
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

void TitleViewer::drawBubbles()
{
	static Texture texture(U"assets/image/particle.png", TextureDesc::Mipped);

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

		texture.resized(r * 0.6).drawAt(x, y, ColorF(Palette::Lightblue, a));
		texture.resized(r * 1.0).drawAt(x, y, ColorF(Palette::Lightblue, a));
	}
}

void TitleViewer::init()
{
	const auto p = RectF(512, 48).setCenter(Vec2(Scene::Center()).movedBy(0.0, Scene::Height() * 0.1));
	addChildViewer<GUIButton>(U"はじめから", [this]() { m_selectedOption = Option::LaunchNewGame; m_closeCurtain.start(); })->setName(U"はじめから")->setViewerRect(p.movedBy(0, 64));
	addChildViewer<GUIButton>(U"つづきから", [this]() { m_selectedOption = Option::ContinueGame; m_closeCurtain.start(); })->setName(U"つづきから")->setViewerRect(p.movedBy(0, 128));
	addChildViewer<GUIButton>(U"エディター", [this]() { m_selectedOption = Option::LaunchEditor; m_closeCurtain.start(); })->setName(U"エディター")->setViewerRect(p.movedBy(0, 192));
	addChildViewer<GUIButton>(U"終了", [this]() { m_selectedOption = Option::Exit; m_closeCurtain.start(); })->setName(U"終了")->setViewerRect(p.movedBy(0, 256));
}

void TitleViewer::update()
{
	// title
	{
		auto te = Transformer2D(Mat3x2::Scale(1.4, Scene::Center()));
		static Font titleFont(256, Typeface::Bold);
		static auto t = 1.0;
		t += 1.0 / 60.0;

		const auto a = 0.25;
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

		drawBubbles();
	}

	// Open Curtain
	{
		if (m_openCurtain.isRunning() && m_openCurtain.update()) m_audio.setVolume(m_openCurtain.getProgress());
		else m_audio.setVolume(1.0);
	}

	// CloseCurtain
	if (m_closeCurtain.isRunning())
	{
		if (m_closeCurtain.update())
		{
			// 自身のインスタンスが削除されるので、returnで処理を飛ばす必要がある。

			switch (m_selectedOption)
			{
			case Option::LaunchNewGame:
				getParentViewer()->addChildViewer<FieldViewer>();
				destroy();
				return;

			case Option::ContinueGame:
				getParentViewer()->addChildViewer<FieldViewer>();
				destroy();
				return;

			case Option::LaunchEditor:
				getParentViewer()->addChildViewer<EditorViewer>();
				destroy();
				return;

			case Option::Exit:
				System::Exit();
				return;
			}
		}

		m_audio.setVolume(m_closeCurtain.getProgress());
	}
}
