#include "MainViewer.h"
#include "TitleViewer.h"
#include "World.h"

void Main()
{
  Profiler::EnableAssetCreationWarning(false);

  // Configの作成
  if (!FileSystem::Exists(U"config.ini"))
    INI().save(U"config.ini");

  // Cursor設定
  if (GeneralSetting::GetInstance().m_touchPanelModeEnabled)
    Cursor::SetDefaultStyle(CursorStyle::Hidden);

  Window::SetTitle(U"SyLife");
  Window::SetStyle(WindowStyle::Sizable);
  Scene::SetResizeMode(ResizeMode::Keep);
  Scene::Resize(GeneralSetting::GetInstance().m_sceneSize);

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
