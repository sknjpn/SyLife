#pragma once
#include <Siv3D.hpp>
#include "lwsock.hpp"
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <memory>
#include <queue>

class User
{
  std::shared_ptr<lwsock::WebSocket> m_socket;
  std::queue<std::string> m_recvQueue;
  bool m_isConnected;
  std::thread m_thread;
  std::mutex m_mtx;

public:
  User(const std::shared_ptr<lwsock::WebSocket> &socket)
      : m_socket(socket), m_isConnected(true), m_thread([this]()
                                                        {
      auto hs = m_socket->recv_req();
      m_socket->send_res();
      while(true)
      {
        auto recv = m_socket->recv_msg_txt();
        std::lock_guard<std::mutex> lock(m_mtx);
        if(recv.second)
        {
          m_isConnected = false;
        }
        else
        {
          m_recvQueue.emplace(recv.first);
        }
      } })
  {
  }

  void send(const std::string &text)
  {
    m_socket->send_msg_txt(text);
  }

  std::optional<std::string> recv(const std::string &text)
  {
    std::lock_guard<std::mutex> lock(m_mtx);

    if (m_recvQueue.empty())
    {
      return std::nullopt;
    }
    else
    {
      const auto result = m_recvQueue.front();
      m_recvQueue.pop();
      return result;
    }
  }
};

class WebSocketServer
{
  lwsock::WebSocket m_socket;
  int m_port;
  std::vector<std::shared_ptr<User>> m_users;
  std::thread m_thread;
  std::mutex m_mtx;

public:
  WebSocketServer(int port)
      : m_socket(lwsock::WebSocket::Mode::SERVER), m_port(port), m_thread([this]()
                                                                          {
  m_socket
        .bind((std::string("ws://localhost:") + std::to_string(m_port)).c_str())
        .listen(128);

  while (true)
  {
    auto user = std::make_shared<User>(std::make_shared<lwsock::WebSocket>(m_socket.accept()));
    std::lock_guard<std::mutex> lock(m_mtx);
    m_users.emplace_back(user);
  } })
  {
  }

  ~WebSocketServer() { m_thread.join(); }

  const std::vector<std::shared_ptr<User>> &getUsers() const { return m_users; }
  const std::mutex &getMutex() const { return m_mtx; }
  std::mutex &getMutex() { return m_mtx; }
};
