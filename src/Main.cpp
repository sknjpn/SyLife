#include "MainViewer.h"
#include "TitleViewer.h"
#include "World.h"
#ifdef SYLIFE_WEB
#include "WebSocket.hpp"
#endif
#ifdef SYLIFE_SERVER
#include "lwsock.hpp"
#include <thread>
void worker(lwsock::WebSocket &&nws)
{
  auto hs = nws.recv_req(); // returned handshake data
  nws.send_res();
  std::string msg = "d e f";
  nws.send_msg_txt(msg);
  auto rcvd = nws.recv_msg_txt();
  Console << Unicode::FromUTF8(rcvd.first);
  // std::cout << rcvd << std::endl;
  nws.send_close(1000);
}
#endif

void Main()
{
#ifdef SYLIFE_SERVER

  lwsock::WebSocket s(lwsock::WebSocket::Mode::SERVER);
  // s.ostream4log(cout);
  s.bind("ws://localhost:3000");
  s.listen(128);

  std::thread th_server([&](){
    while(true)
    {
      auto nws = s.accept();
      auto hs = nws.recv_req();
      Console << Unicode::FromUTF8(hs.first);
      for(const auto& p : hs.second)
      { 
        Console << Unicode::FromUTF8(p.first);
        Console << Unicode::FromUTF8(p.second);
        Console << U"----";
      }

      nws.send_res();
      nws.send_msg_txt("hello world!! from Server!!!");
      auto rcvd = nws.recv_msg_txt();
      Console << Unicode::FromUTF8(rcvd.first);
      nws.send_close(1000); 
    }
  });

  while (System::Update())
  {
    ClearPrint();
    Print << U"Hello";
  }

  s.listen(5);
  std::vector<std::thread> th_set;
  for (int i = 0; i < 3; ++i)
  {
    lwsock::WebSocket nws = s.accept(); // blocking, return new WebSocket object
    auto th = std::thread(worker, std::move(nws));
    th_set.push_back(std::move(th));
  }
  for (auto &th : th_set)
  {
    th.join();
  }
#endif

#ifdef SYLIFE_WEB
  WebSocket ws("ws://localhost:3000");
  ws.SendText("Hello World!");

  int cnt = 0;
  while (System::Update())
  {
    if ((cnt++) % 60 == 0)
      ws.SendText("Hello World!");

    if (ws.hasReceivedText())
      Console << Unicode::FromUTF8(ws.getReceivedTextAndPopFromBuffer());
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
