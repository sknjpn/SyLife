#include <gtest/gtest.h>
#include "sylife/logger/file_sink.h"
#include "sylife/logger/default_formatter.h"
#include <filesystem>
#include <fstream>

using namespace sylife::logger;

class FileSinkTest : public ::testing::Test {
protected:
    void SetUp() override {
        testDir = "test_file_sink";
        std::filesystem::create_directories(testDir);
        
        config.filePath = testDir + "/test.log";
        config.maxFileSize = 1024;
        config.maxFiles = 3;
        config.appendMode = false;
        
        formatter = std::make_shared<DefaultFormatter>(false, false);
    }

    void TearDown() override {
        std::filesystem::remove_all(testDir);
    }

    std::string testDir;
    FileSink::Config config;
    std::shared_ptr<DefaultFormatter> formatter;
};

TEST_F(FileSinkTest, Construction) {
    EXPECT_NO_THROW(FileSink sink(config, formatter));
}

TEST_F(FileSinkTest, FileCreation) {
    FileSink sink(config, formatter);
    
    LogEntry entry{
        .level = LogLevel::Info,
        .message = "Test message",
        .location = std::source_location::current(),
        .timestamp = std::chrono::system_clock::now(),
        .thread_id = std::this_thread::get_id()
    };
    
    sink.write(entry);
    sink.flush();
    
    EXPECT_TRUE(std::filesystem::exists(config.filePath));
}

TEST_F(FileSinkTest, WriteMessage) {
    FileSink sink(config, formatter);
    
    LogEntry entry{
        .level = LogLevel::Info,
        .message = "Test message content",
        .location = std::source_location::current(),
        .timestamp = std::chrono::system_clock::now(),
        .thread_id = std::this_thread::get_id()
    };
    
    sink.write(entry);
    sink.flush();
    
    std::ifstream file(config.filePath);
    std::string content((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    
    EXPECT_TRUE(content.find("Test message content") != std::string::npos);
}

TEST_F(FileSinkTest, ActiveState) {
    FileSink sink(config, formatter);
    
    EXPECT_TRUE(sink.isActive());
    
    sink.setActive(false);
    EXPECT_FALSE(sink.isActive());
    
    sink.setActive(true);
    EXPECT_TRUE(sink.isActive());
}

TEST_F(FileSinkTest, WriteWhenInactive) {
    FileSink sink(config, formatter);
    sink.setActive(false);
    
    LogEntry entry{
        .level = LogLevel::Info,
        .message = "Test message",
        .location = std::source_location::current(),
        .timestamp = std::chrono::system_clock::now(),
        .thread_id = std::this_thread::get_id()
    };
    
    // Should not throw when inactive
    EXPECT_NO_THROW(sink.write(entry));
}

TEST_F(FileSinkTest, FileRotation) {
    config.maxFileSize = 50; // Very small for testing
    FileSink sink(config, formatter);
    
    LogEntry entry{
        .level = LogLevel::Info,
        .message = "This is a long message that should trigger rotation",
        .location = std::source_location::current(),
        .timestamp = std::chrono::system_clock::now(),
        .thread_id = std::this_thread::get_id()
    };
    
    // Write multiple messages to trigger rotation
    for (int i = 0; i < 5; ++i) {
        sink.write(entry);
        sink.flush();
    }
    
    // Check that rotation occurred
    EXPECT_TRUE(std::filesystem::exists(config.filePath));
    EXPECT_TRUE(std::filesystem::exists(config.filePath.string() + ".1"));
}

TEST_F(FileSinkTest, AppendMode) {
    config.appendMode = true;
    
    // Write first message
    {
        FileSink sink(config, formatter);
        LogEntry entry{
            .level = LogLevel::Info,
            .message = "First message",
            .location = std::source_location::current(),
            .timestamp = std::chrono::system_clock::now(),
            .thread_id = std::this_thread::get_id()
        };
        sink.write(entry);
        sink.flush();
    }
    
    // Write second message (should append)
    {
        FileSink sink(config, formatter);
        LogEntry entry{
            .level = LogLevel::Info,
            .message = "Second message",
            .location = std::source_location::current(),
            .timestamp = std::chrono::system_clock::now(),
            .thread_id = std::this_thread::get_id()
        };
        sink.write(entry);
        sink.flush();
    }
    
    std::ifstream file(config.filePath);
    std::string content((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    
    EXPECT_TRUE(content.find("First message") != std::string::npos);
    EXPECT_TRUE(content.find("Second message") != std::string::npos);
}