#include "TitleViewer.h"
#include "Terrain.h"
void Main()
{
	auto tex = TerrainGenerator().MakeTerrainModel(500, 500);
	Array<Vec2>	spots;

	while (System::Update())
	{
		if (KeySpace.down())
		{
			spots.clear();
			TerrainGenerator generator;
			tex = generator.MakeTerrainModel(500, 500);

			for (int i = 0; i < 10000; ++i)
			{
				Vec2 p = RandomVec2(1.0, 1.0);

				if (int(pow(generator.GetHeight(p), 2.0) * 10.0) > Random(10))
					spots.emplace_back(p * 500.0);
			}
		}

		tex->m_texture.draw();

		for (auto& p : spots) Triangle(p, 5.0).draw(Palette::Red);
	}

	return;

	Window::SetTitle(U"SyLife");

	//Graphics::SetFullScreen(true, Size(1920, 1080));
	//Window::SetBaseSize(Size(1920, 1080));

	Window::Resize(Size(1920, 1080));

	g_systemManagerPtr = make_unique<SystemManager>();

	g_viewerManagerPtr->MakeViewer<TitleViewer>();

	while (System::Update())
	{
		g_systemManagerPtr->Update();

		if (KeyEscape.down()) break;
	}
}
