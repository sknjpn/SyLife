#include "TitleViewer.h"
#include "Terrain.h"
void Main()
{
	Window::SetTitle(U"SyLife");

	//Graphics::SetFullScreen(true, Size(1920, 1080));
	//Window::SetBaseSize(Size(1920, 1080));

	Window::Resize(Size(1920, 1080));

	g_systemManagerPtr = make_unique<SystemManager>();

	CursorCamera2D camera;
	camera.setCenter(Vec2::Zero());
	camera.setTargetCenter(Vec2::Zero());
	while (System::Update())
	{
		camera.update();
		auto t = camera.createTransformer();

		g_assetManagerPtr->GetModel<TerrainModel>("Terrain A")->Draw();

		if (KeySpace.down()) g_assetManagerPtr->GetModel<TerrainModel>("Terrain A")->UpdateProperties();
	}

	g_viewerManagerPtr->MakeViewer<TitleViewer>();

	while (System::Update())
	{
		g_systemManagerPtr->Update();

		if (KeyEscape.down()) break;
	}
}
