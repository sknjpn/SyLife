#include "TitleViewer.h"
#include "MainViewer.h"
#include "World.h"
#include "GUIText.h"
#include "GUIValuer.h"
#include "GUIButton.h"
#include "GUICurtain.h"

void TitleViewer::WorldGenerator::generate()
{
	m_noiseSeedX = Random(0xFFFFFFFF);
	m_noiseSeedY = Random(0xFFFFFFFF);

	Point size;
	{
		const auto v = getChildViewer<GUIValuer>(U"sizeValuer");
		size = Point(16, 9) * int(v->getValue() * 10 + 1);
	}

	double waveInterval;
	{
		const auto v = getChildViewer<GUIValuer>(U"waveIntervalValuer");
		waveInterval = Math::Lerp(250.0, 1250.0, v->getValue());
	}

	double element;
	{
		const auto v = getChildViewer<GUIValuer>(U"elementValuer");
		element = v->getValue() * 200.0;
	}

	struct Tile
	{
		double m_element;
		double m_sendRate[3][3];
	};

	Grid<Tile> tiles(size);

	PerlinNoise perlinNoiseX(m_noiseSeedX);
	PerlinNoise perlinNoiseY(m_noiseSeedY);

	for (auto p : step(size))
	{
		//外的圧力比
		const double rx = (p.x - size.x / 2.0) / (size.x / 2.0);
		const double ry = (p.y - size.y / 2.0) / (size.y / 2.0);

		const auto wx = perlinNoiseX.noise(TileLength * (Vec2(p) + Vec2(0.5, 0.5)).x / waveInterval, TileLength * (Vec2(p) + Vec2(0.5, 0.5)).y / waveInterval);
		const auto wy = perlinNoiseY.noise(TileLength * (Vec2(p) + Vec2(0.5, 0.5)).x / waveInterval, TileLength * (Vec2(p) + Vec2(0.5, 0.5)).y / waveInterval);

		// 最大の長さを1とする
		Vec2 waveVelocity = Vec2(Math::Lerp(wx, rx > 0 ? -1.0 : 1.0, EaseInExpo(Abs(rx))), Math::Lerp(wy, ry > 0 ? -1.0 : 1.0, EaseInExpo(Abs(ry)))) / Math::Sqrt2;

		const Vec2 d = waveVelocity * 0.015;
		const double l = 0.01;
		const double w = 1.0 + l * 2;
		const RectF rect = RectF(-l, -l, w, w).movedBy(d);
		const double area = rect.area();
		auto& sendRate = tiles[p].m_sendRate;

		// 初期化
		for (auto point : step(Size(3, 3)))
			sendRate[point.x][point.y] = 0.0;

		// 周囲
		if (rect.tl().x < 0.0) sendRate[0][1] = (-rect.tl().x) * (Min(rect.br().y, 1.0) - Max(rect.tl().y, 0.0)) / area;
		if (rect.tl().y < 0.0) sendRate[1][0] = (-rect.tl().y) * (Min(rect.br().x, 1.0) - Max(rect.tl().x, 0.0)) / area;
		if (rect.br().x > 1.0) sendRate[2][1] = (rect.br().x - 1) * (Min(rect.br().y, 1.0) - Max(rect.tl().y, 0.0)) / area;
		if (rect.br().y > 1.0) sendRate[1][2] = (rect.br().y - 1) * (Min(rect.br().x, 1.0) - Max(rect.tl().x, 0.0)) / area;
		if (rect.tl().x < 0.0 && rect.tl().y < 0.0) sendRate[0][0] = (-rect.tl().x) * (-rect.tl().y) / area;
		if (rect.tl().x < 0.0 && rect.br().y > 1.0) sendRate[0][2] = (-rect.tl().x) * (rect.br().y - 1.0) / area;
		if (rect.br().x > 1.0 && rect.tl().y < 0.0) sendRate[2][0] = (rect.br().x - 1.0) * (-rect.tl().y) / area;
		if (rect.br().x > 1.0 && rect.br().y > 1.0) sendRate[2][2] = (rect.br().x - 1.0) * (rect.br().y - 1.0) / area;

		// 中心
		sendRate[1][1] = 1.0 - sendRate[0][0] - sendRate[1][0] - sendRate[2][0] - sendRate[0][1] - sendRate[2][1] - sendRate[0][2] - sendRate[1][2] - sendRate[2][2];

		// 存在しないところの分を移動
		{
			if (p.x == 0)
			{
				for (int i = 0; i < 3; ++i)
				{
					sendRate[1][i] += sendRate[0][i];
					sendRate[0][i] = 0;
				}
			}
			if (p.y == 0)
			{
				for (int i = 0; i < 3; ++i)
				{
					sendRate[i][1] += sendRate[i][0];
					sendRate[i][0] = 0;
				}
			}
			if (p.x == size.x - 1)
			{
				for (int i = 0; i < 3; ++i)
				{
					sendRate[1][i] += sendRate[2][i];
					sendRate[2][i] = 0;
				}
			}
			if (p.y == size.y - 1)
			{
				for (int i = 0; i < 3; ++i)
				{
					sendRate[i][1] += sendRate[i][2];
					sendRate[i][2] = 0;
				}
			}
		}
	}

	for (auto& t : tiles) t.m_element = element;

	for (int i = 0; i < 2500; ++i)
	{
		{
			Grid<double> elementMap_swap(size);
			Array<ConcurrentTask<void>> tasks;

			for (int ty = 0; ty < size.y; ++ty)
			{
				tasks.emplace_back([&tiles, &elementMap_swap, &size, ty]()
					{
						for (int tx = 0; tx < size.x; ++tx)
						{
							elementMap_swap[ty][tx] = 0.0;

							for (int x = -1; x <= 1; ++x)
								for (int y = -1; y <= 1; ++y)
									if (tx + x != -1 && tx + x != size.x && ty + y != -1 && ty + y != size.y)
										elementMap_swap[ty][tx] += tiles[ty + y][tx + x].m_sendRate[1 - x][1 - y] * tiles[ty + y][tx + x].m_element;
						}
					}
				);
			}

			for (auto& t : tasks)
				while (!t.is_done());

			for (auto p : step(size))
				tiles[p].m_element = elementMap_swap[p];
		}
	}

	{
		Image image(size);

		for (auto p : step(size))
			image[p] = Color(Palette::Palegreen, Min(255, int(tiles[p].m_element * 2.5)));

		m_fieldTexture = Texture(image);
	}
}

void TitleViewer::WorldGenerator::init()
{
	setViewerSize(600, 320);
	setViewerPosInLocal(Scene::Center() - getViewerSize() / 2.0);

	addChildViewer<GUIText>(U"ワールド生成設定", Font(32, Typeface::Heavy), GUIText::Mode::DrawAtCenter)
		->setViewerRectInLocal(5, 5, 580, 40);

	{
		addChildViewer<GUIText>(U"タイルサイズの調整", Font(18, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
			->setName(U"sizeText")
			->setViewerRectInLocal(15, 50, 280, 30);

		addChildViewer<GUIValuer>(0.3)
			->setName(U"sizeValuer")
			->setViewerRectInLocal(15, 80, 150, 20);
	}

	{
		addChildViewer<GUIText>(U"波の間隔の調整", Font(18, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
			->setName(U"waveIntervalText")
			->setViewerRectInLocal(15, 120, 280, 30);

		addChildViewer<GUIValuer>(0.25)
			->setName(U"waveIntervalValuer")
			->setViewerRectInLocal(15, 150, 150, 20);
	}

	{
		addChildViewer<GUIText>(U"平均エレメント量の調整", Font(18, Typeface::Bold), GUIText::Mode::DrawLeftCenter)
			->setName(U"elementText")
			->setViewerRectInLocal(15, 190, 280, 30);

		addChildViewer<GUIValuer>(0.5)
			->setName(U"elementValuer")
			->setViewerRectInLocal(15, 220, 150, 20);
	}

	addChildViewer<GUIButton>([this]() { getParentViewer()->addChildViewer<GUICurtain>(Color(0, 0), Color(11, 22, 33), 0.5, [this]() { onStart(); }); })
		->setViewerRectInLocal(20, 270, 320, 40)
		->addChildViewer<GUIText>(U"開始する！", Font(32, Typeface::Bold));

	if (FileSystem::Exists(U"world/"))
		addChildViewer<GUIButton>([this]() { getParentViewer()->addChildViewer<GUICurtain>(Color(0, 0), Color(11, 22, 33), 0.5, [this]() { onContinue(); }); })
		->setViewerRectInLocal(360, 270, 220, 40)
		->addChildViewer<GUIText>(U"つづきから", Font(32, Typeface::Bold));

	addChildViewer<GUIButton>([this]() { generate(); })
		->setViewerRectInLocal(320, 210, 260, 30)
		->addChildViewer<GUIText>(U"プレビュー", Font(32, Typeface::Bold));

	generate();
}

void TitleViewer::WorldGenerator::update()
{
	setViewerPosInLocal(Scene::Center() - getViewerSize() / 2.0);

	RectF(getViewerSize()).rounded(5).draw(ColorF(Palette::Lightblue)).drawFrame(2.0, 0.0, Palette::Black);

	// size
	{
		const auto v = getChildViewer<GUIValuer>(U"sizeValuer");
		Point size = Point(16, 9) * int(v->getValue() * 10 + 1);

		getChildViewer<GUIText>(U"sizeText")
			->m_text = Format(U"タイルサイズの調整:", size.x, U"x", size.y);
	}

	// waveInterval
	{
		const auto v = getChildViewer<GUIValuer>(U"waveIntervalValuer");
		const auto waveInterval = Math::Lerp(250.0, 1250.0, v->getValue());

		getChildViewer<GUIText>(U"waveIntervalText")
			->m_text = Format(U"波の間隔の調整:", int(waveInterval));
	}

	// element
	{
		const auto v = getChildViewer<GUIValuer>(U"elementValuer");
		const auto element = v->getValue() * 200.0;

		getChildViewer<GUIText>(U"elementText")
			->m_text = Format(U"平均エレメント量の調整:", int(element), U"elm");
	}

	// field
	{
		setDrawPos(310, 55);

		Rect(280, 195).rounded(5).draw(Palette::White).drawFrame(2.0, 0.0, Palette::Black);

		moveDrawPos(20, 10);
		Rect(240, 135).draw(Color(11, 22, 33));

		const ScopedRenderStates2D state(SamplerState::BorderLinear);
		static const PixelShader ps(U"resources/tile" SIV3D_SELECT_SHADER(U".hlsl", U".frag"), { { U"PSConstants2D", 0 } });
		const ScopedCustomShader2D shader(ps);

		m_fieldTexture.resized(240, 135).draw();

	}
}

void TitleViewer::WorldGenerator::onStart()
{
	if (FileSystem::Exists(U"world/"))
		FileSystem::Remove(U"world/");

	World::Make();
	World::GetInstance()->setName(U"New World");
	World::GetInstance()->setTileSize(Point(16, 9) * int(getChildViewer<GUIValuer>(U"sizeValuer")->getValue() * 10 + 1));
	World::GetInstance()->m_waveInterval = Math::Lerp(250.0, 1250.0, getChildViewer<GUIValuer>(U"waveIntervalValuer")->getValue());
	World::GetInstance()->m_elementPerTile = 200.0 * getChildViewer<GUIValuer>(U"elementValuer")->getValue();
	World::GetInstance()->m_perlinNoiseX = PerlinNoise(m_noiseSeedX);
	World::GetInstance()->m_perlinNoiseY = PerlinNoise(m_noiseSeedY);
	World::GetInstance()->init();

	GetRootViewer()->addChildViewer<MainViewer>();

	getParentViewer()->destroy();
}

void TitleViewer::WorldGenerator::onContinue()
{
	World::Load(U"world/");

	GetRootViewer()->addChildViewer<MainViewer>();

	getParentViewer()->destroy();
}
