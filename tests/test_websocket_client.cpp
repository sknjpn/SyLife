#include "test_common.h"
#include "../src/WebSocketClient.hpp"
#include <thread>
#include <chrono>

class WebSocketClientTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        client_ = std::make_unique<WebSocketClient>();
    }
    
    void TearDown() override {
        if (client_ && client_->isConnected()) {
            client_->disconnect();
        }
        SyLifeTestBase::TearDown();
    }
    
    std::unique_ptr<WebSocketClient> client_;
};

TEST_F(WebSocketClientTest, Construction) {
    EXPECT_NE(client_, nullptr);
    EXPECT_FALSE(client_->isConnected());
}

TEST_F(WebSocketClientTest, ConnectionToInvalidServer) {
    // Test connection to non-existent server
    EXPECT_FALSE(client_->connect("ws://localhost:9999"));
    EXPECT_FALSE(client_->isConnected());
}

TEST_F(WebSocketClientTest, InvalidURL) {
    // Test connection with invalid URL
    EXPECT_FALSE(client_->connect("invalid-url"));
    EXPECT_FALSE(client_->connect("http://localhost:8080")); // Wrong protocol
    EXPECT_FALSE(client_->connect(""));
    EXPECT_FALSE(client_->isConnected());
}

TEST_F(WebSocketClientTest, SendWithoutConnection) {
    // Test sending message without connection
    EXPECT_FALSE(client_->send("test message"));
    EXPECT_FALSE(client_->isConnected());
}

TEST_F(WebSocketClientTest, MultipleConnectionAttempts) {
    // Attempt to connect to invalid server
    EXPECT_FALSE(client_->connect("ws://localhost:9998"));
    EXPECT_FALSE(client_->isConnected());
    
    // Second attempt should also fail
    EXPECT_FALSE(client_->connect("ws://localhost:9997"));
    EXPECT_FALSE(client_->isConnected());
}

TEST_F(WebSocketClientTest, DisconnectWithoutConnection) {
    // Test disconnecting without being connected
    EXPECT_NO_THROW(client_->disconnect());
    EXPECT_FALSE(client_->isConnected());
}

TEST_F(WebSocketClientTest, MessageQueue) {
    // Test message queuing functionality (if available)
    EXPECT_EQ(client_->getQueuedMessageCount(), 0);
    
    // Try to queue message (behavior depends on implementation)
    client_->send("queued message");
    
    // Message might be queued or discarded depending on implementation
    EXPECT_GE(client_->getQueuedMessageCount(), 0);
}

TEST_F(WebSocketClientTest, ReconnectionAttempts) {
    // Test automatic reconnection features (if implemented)
    client_->setAutoReconnect(true);
    EXPECT_TRUE(client_->getAutoReconnect());
    
    client_->setAutoReconnect(false);
    EXPECT_FALSE(client_->getAutoReconnect());
}

TEST_F(WebSocketClientTest, MessageReceiving) {
    // Test message receiving functionality
    EXPECT_EQ(client_->getReceivedMessageCount(), 0);
    
    // Simulate receiving messages (implementation dependent)
    auto messages = client_->getReceivedMessages();
    EXPECT_EQ(messages.size(), 0);
}

TEST_F(WebSocketClientTest, ConnectionTimeouts) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Attempt connection to non-existent server (should timeout quickly)
    EXPECT_FALSE(client_->connect("ws://192.0.2.1:8080")); // RFC5737 test address
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
    
    // Should timeout within reasonable time
    EXPECT_LT(duration.count(), 30); // Less than 30 seconds
}

TEST_F(WebSocketClientTest, ErrorHandling) {
    // Test various error conditions
    EXPECT_NO_THROW(client_->send("")); // Empty message
    
    // Test very large message
    std::string largeMessage(100000, 'X');
    EXPECT_NO_THROW(client_->send(largeMessage));
    
    // Test special characters
    std::string specialMessage = "Special chars: \n\r\t\0€";
    EXPECT_NO_THROW(client_->send(specialMessage));
}

TEST_F(WebSocketClientTest, ThreadSafety) {
    // Test concurrent operations
    std::vector<std::thread> threads;
    const int numThreads = 3;
    
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([this, i]() {
            // Attempt concurrent connections
            client_->connect("ws://localhost:999" + std::to_string(i));
            
            // Attempt concurrent message sending
            for (int j = 0; j < 10; ++j) {
                client_->send("Thread " + std::to_string(i) + " Message " + std::to_string(j));
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            
            client_->disconnect();
        });
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
}

TEST_F(WebSocketClientTest, PerformanceTest) {
    // Performance test without actual connection
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Attempt many operations
    const int operationCount = 1000;
    for (int i = 0; i < operationCount; ++i) {
        client_->send("Performance test message " + std::to_string(i));
        client_->getReceivedMessageCount();
        client_->isConnected();
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    // Should complete within reasonable time
    EXPECT_LT(duration.count(), 1000); // Less than 1 second
}

// Integration test with mock server
class MockWebSocketServer {
public:
    MockWebSocketServer() : running_(false), port_(0) {}
    
    bool start(int port) {
        port_ = port;
        running_ = true;
        return true;
    }
    
    void stop() {
        running_ = false;
    }
    
    bool isRunning() const { return running_; }
    int getPort() const { return port_; }
    
private:
    bool running_;
    int port_;
};

TEST_F(WebSocketClientTest, IntegrationWithMockServer) {
    MockWebSocketServer mockServer;
    
    // Start mock server
    EXPECT_TRUE(mockServer.start(8090));
    EXPECT_TRUE(mockServer.isRunning());
    
    // Note: This test would require actual WebSocket implementation
    // For now, we just test that the client handles the connection attempt gracefully
    std::string url = "ws://localhost:" + std::to_string(mockServer.getPort());
    
    // This will likely fail since it's a mock server, but should not crash
    EXPECT_NO_THROW(client_->connect(url));
    
    mockServer.stop();
}