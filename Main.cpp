#include "MainViewer.h"
#include "TitleViewer.h"
#include "World.h"

void Main() {
  // Configの作成
  if (!FileSystem::Exists(U"config.ini"))
    INI().save(U"config.ini");

  Profiler::EnableAssetCreationWarning(false);

  const Array<FilePathView> musicFiles = {
    U"天のきざはし",
    U"沈む。",
    U"かみさまのゆりかご",
    U"真相探求",
    U"安らぎと微睡み"
  };

  for (const auto& musicFile : musicFiles)
    if (!FileSystem::Exists(U"resources/music/" + musicFile + U".mp3"))
      GeneralSetting::GetInstance().m_audioEnabled = false;

  // loadBGM
  if (GeneralSetting::GetInstance().m_audioEnabled) {
#if defined(_WIN32) && defined(USE_MUSIC_RESOURCE_FILES)

    for (const auto& musicFile : musicFiles)
      AudioAsset::Register(musicFile, Resource(U"resources/music/" + musicFile + U".mp3"));
#else
    for (const auto& musicFile : musicFiles)
      AudioAsset::Register(musicFile, U"resources/music/" + musicFile + U".mp3");
#endif

    for (const auto& musicFile : musicFiles)
      AudioAsset::LoadAsync(musicFile);
  }

  // Cursor設定
  if (GeneralSetting::GetInstance().m_touchPanelModeEnabled)
    Cursor::SetDefaultStyle(CursorStyle::Hidden);

  Window::SetTitle(U"SyLife");
  Window::SetStyle(WindowStyle::Sizable);
  Scene::SetResizeMode(ResizeMode::Keep);
  Scene::Resize(2560, 1440);

  // Window設定
  if (GeneralSetting::GetInstance().m_fullScreenEnabled) {
    Window::SetFullscreen(true);
  } else {
    Window::Maximize();
  }

  if (GeneralSetting::GetInstance().m_runTitleEnabled) {
    EasyViewer::GetRootViewer()->addChildViewer<TitleViewer>();
  } else {
    // world生成
    if (FileSystem::Exists(U"world/")) {
      World::Load(U"world/");
    } else {
      World::Make();
      World::GetInstance()->setName(U"New World");
    }

    EasyViewer::GetRootViewer()->addChildViewer<MainViewer>();
  }

  EasyViewer::Run();

  GeneralSetting::GetInstance().save();
}
