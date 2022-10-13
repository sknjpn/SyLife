#include "WebSocketClient.hpp"

#ifdef SYLIFE_WEB
#include <Siv3D.hpp>

EM_BOOL WebSocketClient::WebSocketOpen(int eventType, const EmscriptenWebSocketOpenEvent *e, void *userData)
{
  WebSocketClient *socket = (WebSocketClient *)userData;

  Console << U"WebSocketClient Open";

  socket->m_hasConnection = true;

  if (!socket->m_bufferSend.empty())
  {
    for (const auto &buffer : socket->m_bufferSend)
      emscripten_websocket_send_utf8_text(socket->m_socket, buffer.c_str());

    socket->m_bufferSend.clear();
  }

  return 0;
}

EM_BOOL WebSocketClient::WebSocketClose(int eventType, const EmscriptenWebSocketCloseEvent *e, void *userData)
{
  WebSocketClient *socket = (WebSocketClient *)userData;

  Console << U"WebSocketClient Close";

  socket->m_hasConnection = false;

  return 0;
}

EM_BOOL WebSocketClient::WebSocketError(int eventType, const EmscriptenWebSocketErrorEvent *e, void *userData)
{
  WebSocketClient *socket = (WebSocketClient *)userData;

  Console << U"WebSocketClient Error eventType=" << eventType;

  socket->m_hasConnection = false;

  return 0;
}

EM_BOOL WebSocketClient::WebSocketMessage(int eventType, const EmscriptenWebSocketMessageEvent *e, void *userData)
{
  WebSocketClient *socket = (WebSocketClient *)userData;

  Console << U"WebSocket Message numBytes=" << e->numBytes;
  Console << U"Text:" << Unicode::FromUTF8(std::string((char *)e->data));

  socket->m_bufferRecv.emplace_back(std::string((char *)e->data));

  /*if (e->isText)
  {
      Console << U"Text:" << Unicode::FromUTF8(std::string((char *)e->data));
  }
  else
  {
      const char numberToChar[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

      std::string text;
      for (int i = 0; i < e->numBytes; ++i)
      {
          text.push_back(numberToChar[*(e->data + i) >> 4]);
          text.push_back(numberToChar[*(e->data + i) & 0x0F]);
          text.push_back(' ');
      }

      Console << U"Binary:" << Unicode::FromUTF8(text);
  }*/

  return 0;
}

WebSocketClient::WebSocketClient(const char *url)
{
  EmscriptenWebSocketCreateAttributes attributes;
  emscripten_websocket_init_create_attributes(&attributes);
  attributes.url = url;

  m_socket = emscripten_websocket_new(&attributes);
  emscripten_websocket_set_onopen_callback(m_socket, this, WebSocketOpen);
  emscripten_websocket_set_onclose_callback(m_socket, this, WebSocketClose);
  emscripten_websocket_set_onerror_callback(m_socket, this, WebSocketError);
  emscripten_websocket_set_onmessage_callback(m_socket, this, WebSocketMessage);
}

void WebSocketClient::sendText(const std::string &text)
{
  if (m_hasConnection)
  {
    emscripten_websocket_send_utf8_text(m_socket, text.c_str());
  }
  else
  {
    m_bufferSend.emplace_back(text);
  }
}

std::optional<std::string> WebSocketClient::recvText()
{
  if (m_bufferRecv.empty())
    return std::nullopt;

  std::string text = m_bufferRecv.front();
  m_bufferRecv.erase(m_bufferRecv.begin());
  return text;
}

#endif
