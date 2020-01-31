#include "TitleViewer.h"
#include "GUICurtain.h"
#include "MainViewer.h"
#include "GUIButton.h"
#include "GUIText.h"
#include "GUIMusicBox.h"
#include "World.h"

void TitleViewer::updateBubbles()
{
	while (RandomBool(0.5))
	{
		auto& b = m_bubbles.emplace_back();

		b.m_position = Vec3(120.0 * Random(-1.0, 1.0), -100.0, 150.0 + 120.0 * Random(-1.0, 1.0));
	}

	static PerlinNoise noise1(Random(0xFFFFFFFF));
	static PerlinNoise noise2(Random(0xFFFFFFFF));
	static Vec3 liner(0, 0, 0);

	{
		const int numThread = 12;
		Array<ConcurrentTask<void>> tasks;

		for (int i = 0; i < numThread; ++i)
		{
			tasks.emplace_back([this, i, numThread]()
				{
					for (int j = i; j < m_bubbles.size(); j += numThread)
					{
						m_bubbles[j].m_timer += 2.0;
						m_bubbles[j].m_position.x += 0.30 * noise1.noise(m_bubbles[j].m_position * 0.02 + liner);
						m_bubbles[j].m_position.y += 0.30 * Random(0.25, 1.0);
						m_bubbles[j].m_position.z += 0.30 * noise2.noise(m_bubbles[j].m_position * 0.02 + liner);
					}
				});
		}

		for (auto& t : tasks)
			while (!t.is_done());
	}

	liner.moveBy(0, 0, 0.01);

	m_bubbles.remove_if([](const auto& b) { return b.m_timer > 1800.0; });
}

void TitleViewer::drawBubbles()
{
	//static Texture texture(U"resources/image/particle.png", TextureDesc::Mipped);

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
		auto r = 2000.0 / p.length() * Min(b.m_timer / 1000.0, 1.0) * 15.0;
		auto a = Min((1800.0 - b.m_timer) / 500.0, 1.0) * 0.1;

		Circle(x, y, r * 0.5).draw(ColorF(Palette::Lightblue, a));
		//texture.resized(r * 1.0).drawAt(x, y, ColorF(Palette::Lightblue, a));
	}
}

void TitleViewer::runNew()
{
	for (const auto& child : getChildViewers<GUIButton>())
		child->destroy();

	addChildViewer<WorldGenerator>();
}

void TitleViewer::runContinue()
{
	// world生成
	if (FileSystem::Exists(U"world/"))
	{
		World::Load(U"world/");
	}
	else
	{
		World::Make();
		World::GetInstance()->setName(U"New World");
	}

	getParentViewer()->addChildViewer<MainViewer>();

	destroy();
}

void TitleViewer::init()
{
	if (GeneralSetting::GetInstance().m_audioEnabled)
		addChildViewer<GUIMusicBox>(U"天のきざはし");

	const auto p = RectF(500, 50).setCenter(Vec2(Scene::Center()).movedBy(0.0, Scene::Height() * 0.2));

	addChildViewer<GUIButton>([this]() { runNew(); })
		->setViewerRectInLocal(p.movedBy(0, 0))
		->addChildViewer<GUIText>(U"はじめから", Font(40, Typeface::Bold));

	addChildViewer<GUIButton>([this]() { addChildViewer<GUICurtain>(Color(0, 0), Color(11, 22, 33), 0.5, [this]() { runContinue(); }); }, FileSystem::Exists(U"world/"))
		->setViewerRectInLocal(p.movedBy(0, 75))
		->addChildViewer<GUIText>(U"つづきから", Font(40, Typeface::Bold));

	addChildViewer<GUIButton>([this]() { /*getParentViewer()->addChildViewer<EditorViewer>();*/ destroy(); }, false)
		->setViewerRectInLocal(p.movedBy(0, 150))
		->addChildViewer<GUIText>(U"エディター", Font(40, Typeface::Bold));

	addChildViewer<GUIButton>([this]() { addChildViewer<GUICurtain>(Color(0, 0), Color(11, 22, 33), 0.5, [this]() { System::Exit(); }); })
		->setViewerRectInLocal(p.movedBy(0, 225))
		->addChildViewer<GUIText>(U"終了", Font(40, Typeface::Bold));

	// OpenCurtain
	addChildViewer<GUICurtain>(Color(11, 22, 33), Color(0, 0), 0.5);
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
		updateBubbles();

		drawBubbles();
	}
}
