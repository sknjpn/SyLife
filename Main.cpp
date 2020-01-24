#include "MainViewer.h"
#include "TitleViewer.h"
#include "World.h"

void Main()
{
	INIData ini(U"config.ini");

	Window::SetTitle(U"SyLife");
	Window::SetStyle(WindowStyle::Sizable);
	Scene::Resize(1920, 1080);

	// Window設定
	if (ini.getOr<bool>(U"General", U"FullScreen", false))
	{
		if (Window::SetFullscreen(true))
			Window::Maximize();
	}
	else
	{
		Window::Maximize();
	}

	if (ini.getOr<bool>(U"General", U"RunTitle", true))
	{
		EasyViewer::GetRootViewer()->addChildViewer<TitleViewer>();
	}
	else
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

		EasyViewer::GetRootViewer()->addChildViewer<MainViewer>();
	}

	EasyViewer::Run();

	if (World::GetInstance())
		World::GetInstance()->save();
}
