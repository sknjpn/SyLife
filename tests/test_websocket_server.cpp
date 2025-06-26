#include "test_common.h"
#include "../src/WebSocketServer.hpp"
#include <thread>
#include <chrono>

class WebSocketServerTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        server_ = std::make_unique<WebSocketServer>();
    }
    
    void TearDown() override {
        if (server_) {
            server_->stop();
        }
        SyLifeTestBase::TearDown();
    }
    
    std::unique_ptr<WebSocketServer> server_;
};

TEST_F(WebSocketServerTest, Construction) {
    EXPECT_NE(server_, nullptr);
    EXPECT_FALSE(server_->isRunning());
}

TEST_F(WebSocketServerTest, StartAndStop) {
    // Test server start
    EXPECT_TRUE(server_->start(8080));
    EXPECT_TRUE(server_->isRunning());
    
    // Allow some time for server to initialize
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Test server stop
    server_->stop();
    EXPECT_FALSE(server_->isRunning());
}

TEST_F(WebSocketServerTest, InvalidPort) {
    // Test starting with invalid port
    EXPECT_FALSE(server_->start(0));
    EXPECT_FALSE(server_->start(65536));
    EXPECT_FALSE(server_->isRunning());
}

TEST_F(WebSocketServerTest, MultipleStartAttempts) {
    EXPECT_TRUE(server_->start(8081));
    EXPECT_TRUE(server_->isRunning());
    
    // Second start attempt should fail or be ignored
    bool secondStart = server_->start(8082);
    EXPECT_FALSE(secondStart); // Should not be able to start twice
    
    server_->stop();
}

TEST_F(WebSocketServerTest, MessageHandling) {
    EXPECT_TRUE(server_->start(8083));
    
    // Test broadcasting message
    std::string testMessage = "Hello, WebSocket!";
    EXPECT_NO_THROW(server_->broadcast(testMessage));
    
    server_->stop();
}

TEST_F(WebSocketServerTest, UserManagement) {
    EXPECT_TRUE(server_->start(8084));
    
    // Test user count (should be 0 initially)
    EXPECT_EQ(server_->getUserCount(), 0);
    
    // Test maximum users limit
    EXPECT_GT(server_->getMaxUsers(), 0);
    
    server_->stop();
}

// Mock client for testing
class MockWebSocketClient {
public:
    MockWebSocketClient() : connected_(false) {}
    
    bool connect(const std::string& url) {
        // Simulate connection attempt
        connected_ = true;
        return true;
    }
    
    void disconnect() {
        connected_ = false;
    }
    
    bool send(const std::string& message) {
        if (!connected_) return false;
        lastSentMessage_ = message;
        return true;
    }
    
    bool isConnected() const { return connected_; }
    const std::string& getLastSentMessage() const { return lastSentMessage_; }
    
private:
    bool connected_;
    std::string lastSentMessage_;
};

TEST_F(WebSocketServerTest, ClientServerInteraction) {
    EXPECT_TRUE(server_->start(8085));
    
    // Allow server to start
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Create mock client
    MockWebSocketClient client;
    EXPECT_TRUE(client.connect("ws://localhost:8085"));
    
    // Simulate some interaction
    std::string testMessage = "Test message from client";
    EXPECT_TRUE(client.send(testMessage));
    EXPECT_EQ(client.getLastSentMessage(), testMessage);
    
    client.disconnect();
    server_->stop();
}

TEST_F(WebSocketServerTest, ErrorHandling) {
    // Test error conditions
    EXPECT_NO_THROW(server_->broadcast("message")); // Should handle gracefully when not running
    
    // Test invalid operations
    EXPECT_FALSE(server_->isRunning());
    EXPECT_EQ(server_->getUserCount(), 0);
    
    // Start server and test error conditions
    EXPECT_TRUE(server_->start(8086));
    
    // Test sending empty message
    EXPECT_NO_THROW(server_->broadcast(""));
    
    // Test sending very large message
    std::string largeMessage(10000, 'A');
    EXPECT_NO_THROW(server_->broadcast(largeMessage));
    
    server_->stop();
}

TEST_F(WebSocketServerTest, ThreadSafety) {
    EXPECT_TRUE(server_->start(8087));
    
    // Test concurrent access
    std::vector<std::thread> threads;
    const int numThreads = 5;
    const int messagesPerThread = 10;
    
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([this, i, messagesPerThread]() {
            for (int j = 0; j < messagesPerThread; ++j) {
                std::string message = "Thread " + std::to_string(i) + " Message " + std::to_string(j);
                server_->broadcast(message);
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    server_->stop();
}

TEST_F(WebSocketServerTest, PerformanceTest) {
    EXPECT_TRUE(server_->start(8088));
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Send many messages
    const int messageCount = 1000;
    for (int i = 0; i < messageCount; ++i) {
        std::string message = "Performance test message " + std::to_string(i);
        server_->broadcast(message);
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    // Should complete within reasonable time
    EXPECT_LT(duration.count(), 5000); // Less than 5 seconds
    
    server_->stop();
}