#include "MainViewer.h"
#include "World.h"

void Main()
{
	Window::SetTitle(U"SyLife");
	Window::Resize(1920, 1080);
	Window::SetStyle(WindowStyle::Sizable);
	Window::Maximize();

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

	World::GetInstance()->save();
}
