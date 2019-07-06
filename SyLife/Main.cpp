#include "TitleViewer.h"
#include "Terrain.h"
void Main()
{
	Window::SetTitle(U"SyLife");

	//Graphics::SetFullScreen(true, Size(1920, 1080));
	//Window::SetBaseSize(Size(1920, 1080));

	Window::Resize(Size(1024, 1024));

	g_systemManagerPtr = make_unique<SystemManager>();

	g_terrainManagerPtr->SetTerrainModel(g_assetManagerPtr->GetModel<TerrainModel>("Terrain A"));

	g_viewerManagerPtr->MakeViewer<TitleViewer>();

	while (System::Update())
	{
		g_systemManagerPtr->Update();

		if (KeyEscape.down()) break;
	}
}
