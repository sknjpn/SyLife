#include "MainViewer.h"
#include "World.h"

void Main()
{
	Window::SetTitle(U"SyLife");
	Window::SetStyle(WindowStyle::Sizable);
	Scene::Resize(1920, 1080);
	Window::Maximize();
	//Window::SetFullscreen(true);
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

	Viewer::GetRootViewer()->addChildViewer<MainViewer>();
	Viewer::Run();

	//World::GetInstance()->save();
}
