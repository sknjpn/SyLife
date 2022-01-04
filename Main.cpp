#include "MainViewer.h"
#include "TitleViewer.h"
#include "World.h"

void Main()
{
	// Configの作成
	if (!FileSystem::Exists(U"config.ini"))
		INI().save(U"config.ini");

	Profiler::EnableAssetCreationWarning(false);

	// loadBGM
	if (GeneralSetting::GetInstance().m_audioEnabled)
	{
		// TODO:Enable Asset Load
/*#ifdef _WIN32
		AudioAsset::Register(U"天のきざはし", Resource(U"resources/music/天のきざはし.mp3"), AssetParameter::LoadAsync());
		AudioAsset::Register(U"沈む。", Resource(U"resources/music/沈む。.mp3"), AssetParameter::LoadAsync());
		AudioAsset::Register(U"かみさまのゆりかご", Resource(U"resources/music/かみさまのゆりかご.mp3"), AssetParameter::LoadAsync());
		AudioAsset::Register(U"真相探求", Resource(U"resources/music/真相探求.mp3"), AssetParameter::LoadAsync());
		AudioAsset::Register(U"安らぎと微睡み", Resource(U"resources/music/安らぎと微睡み.mp3"), AssetParameter::LoadAsync());
#else
		AudioAsset::Register(U"天のきざはし", U"resources/music/天のきざはし.mp3", AssetParameter::LoadAsync());
		AudioAsset::Register(U"沈む。", U"resources/music/沈む。.mp3", AssetParameter::LoadAsync());
		AudioAsset::Register(U"かみさまのゆりかご", U"resources/music/かみさまのゆりかご.mp3", AssetParameter::LoadAsync());
		AudioAsset::Register(U"真相探求", U"resources/music/真相探求.mp3", AssetParameter::LoadAsync());
		AudioAsset::Register(U"安らぎと微睡み", U"resources/music/安らぎと微睡み.mp3", AssetParameter::LoadAsync());
#endif*/
	}

	// Cursor設定
	if (GeneralSetting::GetInstance().m_touchPanelModeEnabled)
		Cursor::SetDefaultStyle(CursorStyle::Hidden);

	Window::SetTitle(U"SyLife");
	Window::SetStyle(WindowStyle::Sizable);
	Scene::Resize(1920, 1080);

	// Window設定
	if (GeneralSetting::GetInstance().m_fullScreenEnabled)
	{
		Window::SetFullscreen(true);
	}
	else
	{
		Window::Maximize();
	}

	if (GeneralSetting::GetInstance().m_runTitleEnabled)
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

	GeneralSetting::GetInstance().save();
}
