#pragma once

#ifdef SYLIFE_WEB
#include <vector>
#include <string>
#include <optional>
#include <emscripten/websocket.h>

class WebSocketClient
{
    static EM_BOOL WebSocketOpen(int eventType, const EmscriptenWebSocketOpenEvent *e, void *socket);
    static EM_BOOL WebSocketClose(int eventType, const EmscriptenWebSocketCloseEvent *e, void *socket);
    static EM_BOOL WebSocketError(int eventType, const EmscriptenWebSocketErrorEvent *e, void *socket);
    static EM_BOOL WebSocketMessage(int eventType, const EmscriptenWebSocketMessageEvent *e, void *socket);

    EMSCRIPTEN_WEBSOCKET_T m_socket;

    bool m_hasConnection = false;
    std::vector<std::string> m_bufferSend;
    std::vector<std::string> m_bufferRecv;

  public:
    WebSocketClient(const char* url);

    void sendText(const std::string& text);

    std::optional<std::string> recvText();
};
#endif
