#pragma once
#include <Siv3D.hpp>
#include "lwsock.hpp"
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <memory>

struct WebSocketServer
{
  struct User
  {
    User(const std::shared_ptr<lwsock::WebSocket>& conn)
        : m_conn(conn)
    {
      auto hs = m_conn->recv_req();
      Console << Unicode::FromUTF8(hs.first);
      for (const auto &p : hs.second)
      {
        Console << Unicode::FromUTF8(p.first);
        Console << Unicode::FromUTF8(p.second);
        Console << U"----";
      }
      m_conn->send_res();

      m_thread = std::thread([this]()
                             {
        while (true)
        {
          auto rcvd = m_conn->recv_msg_txt();
          if(rcvd.second)
          {
            Console << U"Disconnected";
            break;
          }
          std::lock_guard<std::mutex> lock(m_mtx);
          m_recv.emplace_back(rcvd.first);
          Console << Unicode::FromUTF8(rcvd.first);
        } });
    }
    ~User() { m_thread.join(); }

    std::shared_ptr<lwsock::WebSocket> m_conn;
    std::vector<std::string> m_recv;
    std::thread m_thread;
    std::mutex m_mtx;

    void send(const std::string &text)
    {
      m_conn->send_msg_txt("hello world!! from Server!!!");
    }
  };

  lwsock::WebSocket m_server;
  std::thread m_thread;
  std::vector<std::shared_ptr<User>> m_users;
  std::mutex m_mtx;

public:
  WebSocketServer(int port)
      : m_server(lwsock::WebSocket::Mode::SERVER)
  {
    m_server
        .bind((std::string("ws://localhost:") + std::to_string(port)).c_str())
        .listen(128);

    m_thread = std::thread([this]()
                           {
  while (true)
  {
    auto user = std::make_shared<User>(std::make_shared<lwsock::WebSocket>(m_server.accept()));
    std::lock_guard<std::mutex> lock(m_mtx);
    m_users.emplace_back(user);
  } });
  }

  ~WebSocketServer() { m_thread.join(); }
};
