#include <gtest/gtest.h>
#include "sylife/logger/multi_sink_logger.h"
#include "sylife/logger/console_sink.h"
#include "sylife/logger/file_sink.h"
#include "sylife/logger/default_formatter.h"
#include "sylife/di/container.h"
#include <filesystem>
#include <fstream>

using namespace sylife::logger;
using namespace sylife::di;

class LoggerIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        testDir = "test_logs";
        std::filesystem::create_directories(testDir);
    }

    void TearDown() override {
        std::filesystem::remove_all(testDir);
    }

    std::string testDir;
};

TEST_F(LoggerIntegrationTest, CompleteLoggingWorkflow) {
    // Create a multi-sink logger with console and file output
    auto logger = std::make_unique<MultiSinkLogger>(LogLevel::Debug);
    
    // Create and configure console sink
    auto consoleSink = std::make_shared<ConsoleSink>();
    consoleSink->setColorOutput(false); // Disable colors for testing
    
    // Create and configure file sink
    FileSink::Config fileConfig;
    fileConfig.filePath = testDir + "/test.log";
    fileConfig.maxFileSize = 1024; // Small size for testing rotation
    fileConfig.maxFiles = 3;
    auto fileSink = std::make_shared<FileSink>(fileConfig);
    
    // Add sinks to logger
    logger->addSink(consoleSink);
    logger->addSink(fileSink);
    
    // Test logging at different levels
    logger->debug("Debug message");
    logger->info("Info message");
    logger->warning("Warning message");
    logger->error("Error message");
    logger->critical("Critical message");
    
    // Flush to ensure all messages are written
    logger->flush();
    
    // Verify file was created and contains expected content
    EXPECT_TRUE(std::filesystem::exists(fileConfig.filePath));
    
    std::ifstream logFile(fileConfig.filePath);
    std::string content((std::istreambuf_iterator<char>(logFile)),
                       std::istreambuf_iterator<char>());
    
    EXPECT_TRUE(content.find("Debug message") != std::string::npos);
    EXPECT_TRUE(content.find("Info message") != std::string::npos);
    EXPECT_TRUE(content.find("Warning message") != std::string::npos);
    EXPECT_TRUE(content.find("Error message") != std::string::npos);
    EXPECT_TRUE(content.find("Critical message") != std::string::npos);
}

TEST_F(LoggerIntegrationTest, DependencyInjectionIntegration) {
    // Create DI container
    Container container;
    
    // Register logger components
    auto formatter = std::make_shared<DefaultFormatter>(false, true);
    auto consoleSink = std::make_shared<ConsoleSink>(formatter, false);
    
    FileSink::Config fileConfig;
    fileConfig.filePath = testDir + "/di_test.log";
    auto fileSink = std::make_shared<FileSink>(fileConfig, formatter);
    
    // Register logger factory
    container.registerFactory<ILogger>([consoleSink, fileSink]() {
        auto logger = std::make_shared<MultiSinkLogger>(LogLevel::Info);
        logger->addSink(consoleSink);
        logger->addSink(fileSink);
        return logger;
    });
    
    // Resolve logger from container
    auto logger = container.resolve<ILogger>();
    EXPECT_NE(logger, nullptr);
    
    // Test logging through DI-resolved logger
    logger->info("DI test message");
    logger->flush();
    
    // Verify file was created
    EXPECT_TRUE(std::filesystem::exists(fileConfig.filePath));
    
    std::ifstream logFile(fileConfig.filePath);
    std::string content((std::istreambuf_iterator<char>(logFile)),
                       std::istreambuf_iterator<char>());
    
    EXPECT_TRUE(content.find("DI test message") != std::string::npos);
    
    // Test singleton behavior
    auto logger2 = container.resolve<ILogger>();
    EXPECT_EQ(logger, logger2); // Should be the same instance
}

TEST_F(LoggerIntegrationTest, FileRotationIntegration) {
    FileSink::Config config;
    config.filePath = testDir + "/rotation_test.log";
    config.maxFileSize = 100; // Very small for testing
    config.maxFiles = 3;
    
    auto fileSink = std::make_shared<FileSink>(config);
    auto logger = std::make_unique<MultiSinkLogger>(LogLevel::Debug);
    logger->addSink(fileSink);
    
    // Write enough messages to trigger rotation
    for (int i = 0; i < 10; ++i) {
        logger->info("This is a test message " + std::to_string(i) + 
                    " that should trigger file rotation due to size limits");
        logger->flush();
    }
    
    // Check that rotation occurred
    EXPECT_TRUE(std::filesystem::exists(config.filePath));
    EXPECT_TRUE(std::filesystem::exists(config.filePath.string() + ".1"));
}

TEST_F(LoggerIntegrationTest, MultithreadedLogging) {
    auto logger = std::make_unique<MultiSinkLogger>(LogLevel::Debug);
    
    FileSink::Config config;
    config.filePath = testDir + "/multithread_test.log";
    auto fileSink = std::make_shared<FileSink>(config);
    logger->addSink(fileSink);
    
    std::vector<std::thread> threads;
    const int numThreads = 5;
    const int messagesPerThread = 10;
    
    // Create multiple threads that log simultaneously
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&logger, i, messagesPerThread]() {
            for (int j = 0; j < messagesPerThread; ++j) {
                logger->info("Thread " + std::to_string(i) + 
                           " message " + std::to_string(j));
            }
        });
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    logger->flush();
    
    // Verify all messages were logged
    std::ifstream logFile(config.filePath);
    std::string content((std::istreambuf_iterator<char>(logFile)),
                       std::istreambuf_iterator<char>());
    
    // Count occurrences of "Thread" to verify all messages were logged
    size_t count = 0;
    size_t pos = 0;
    while ((pos = content.find("Thread", pos)) != std::string::npos) {
        ++count;
        ++pos;
    }
    
    EXPECT_EQ(count, numThreads * messagesPerThread);
}

TEST_F(LoggerIntegrationTest, PerformanceTest) {
    auto logger = std::make_unique<MultiSinkLogger>(LogLevel::Info);
    
    FileSink::Config config;
    config.filePath = testDir + "/performance_test.log";
    auto fileSink = std::make_shared<FileSink>(config);
    logger->addSink(fileSink);
    
    const int messageCount = 1000;
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < messageCount; ++i) {
        logger->info("Performance test message " + std::to_string(i));
    }
    
    logger->flush();
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Should be able to log 1000 messages in less than 1 second
    EXPECT_LT(duration.count(), 1000);
    
    // Verify all messages were logged
    std::ifstream logFile(config.filePath);
    std::string content((std::istreambuf_iterator<char>(logFile)),
                       std::istreambuf_iterator<char>());
    
    size_t count = 0;
    size_t pos = 0;
    while ((pos = content.find("Performance test message", pos)) != std::string::npos) {
        ++count;
        ++pos;
    }
    
    EXPECT_EQ(count, messageCount);
}