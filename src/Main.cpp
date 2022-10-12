#include "MainViewer.h"
#include "TitleViewer.h"
#include "World.h"
#ifdef SYLIFE_WEB
#include "WebSocketClient.hpp"
#endif
#ifdef SYLIFE_SERVER
#include "WebSocketServer.hpp"
#endif

void Main()
{
#ifdef SYLIFE_SERVER
  WebSocketServer wss(3000);
  while (System::Update())
  {
    std::lock_guard<std::mutex> lock(wss.m_mtx);
    ClearPrint();
    Print << wss.m_users.size();
  }
#endif

#ifdef SYLIFE_WEB
  WebSocketClient ws("ws://localhost:3000");
  ws.sendText("Hello World!");

  int cnt = 0;
  while (System::Update())
  {
    if ((cnt++) % 60 == 0)
      ws.sendText("Hello World!");

    while(auto recv = ws.recvText())
      Console << Unicode::FromUTF8(recv.value());
  }
#endif

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
