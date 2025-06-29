#include <gtest/gtest.h>
#include "sylife/logger/default_formatter.h"
#include <thread>

using namespace sylife::logger;

class DefaultFormatterTest : public ::testing::Test {
protected:
    void SetUp() override {
        formatter = std::make_unique<DefaultFormatter>(false, true);
        
        entry = LogEntry{
            .level = LogLevel::Info,
            .message = "Test message",
            .location = std::source_location::current(),
            .timestamp = std::chrono::system_clock::now(),
            .thread_id = std::this_thread::get_id()
        };
    }

    std::unique_ptr<DefaultFormatter> formatter;
    LogEntry entry;
};

TEST_F(DefaultFormatterTest, BasicFormatting) {
    auto result = formatter->format(entry);
    
    EXPECT_TRUE(result.find("Test message") != std::string::npos);
    EXPECT_TRUE(result.find("[INFO]") != std::string::npos);
}

TEST_F(DefaultFormatterTest, IncludeLocation) {
    formatter->setIncludeLocation(true);
    auto result = formatter->format(entry);
    
    EXPECT_TRUE(result.find(".cpp:") != std::string::npos);
}

TEST_F(DefaultFormatterTest, ExcludeLocation) {
    formatter->setIncludeLocation(false);
    auto result = formatter->format(entry);
    
    EXPECT_FALSE(result.find(".cpp:") != std::string::npos);
}

TEST_F(DefaultFormatterTest, IncludeThreadId) {
    formatter->setIncludeThreadId(true);
    auto result = formatter->format(entry);
    
    // Should contain some thread ID representation
    EXPECT_GT(result.length(), 0);
}

TEST_F(DefaultFormatterTest, ExcludeThreadId) {
    formatter->setIncludeThreadId(false);
    auto result1 = formatter->format(entry);
    
    formatter->setIncludeThreadId(true);
    auto result2 = formatter->format(entry);
    
    // Result with thread ID should be longer
    EXPECT_GT(result2.length(), result1.length());
}

TEST_F(DefaultFormatterTest, DifferentLogLevels) {
    auto levels = {LogLevel::Debug, LogLevel::Info, LogLevel::Warning, 
                   LogLevel::Error, LogLevel::Critical};
    
    for (auto level : levels) {
        entry.level = level;
        auto result = formatter->format(entry);
        
        auto levelStr = logLevelToString(level);
        EXPECT_TRUE(result.find(levelStr) != std::string::npos);
    }
}

TEST_F(DefaultFormatterTest, CustomDateTimeFormat) {
    formatter->setDateTimeFormat("%H:%M:%S");
    auto result = formatter->format(entry);
    
    // Should contain time in HH:MM:SS format
    EXPECT_TRUE(result.find(":") != std::string::npos);
}

TEST_F(DefaultFormatterTest, EmptyMessage) {
    entry.message = "";
    auto result = formatter->format(entry);
    
    // Should still contain level and timestamp
    EXPECT_TRUE(result.find("[INFO]") != std::string::npos);
}

TEST_F(DefaultFormatterTest, LongMessage) {
    entry.message = std::string(1000, 'x');
    auto result = formatter->format(entry);
    
    EXPECT_TRUE(result.find(std::string(1000, 'x')) != std::string::npos);
}

TEST_F(DefaultFormatterTest, SpecialCharacters) {
    entry.message = "Message with\nnewlines\tand\ttabs";
    auto result = formatter->format(entry);
    
    EXPECT_TRUE(result.find("Message with\nnewlines\tand\ttabs") != std::string::npos);
}

TEST_F(DefaultFormatterTest, ThreadSafety) {
    std::vector<std::thread> threads;
    std::vector<std::string> results(10);
    
    // Multiple threads formatting simultaneously
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([this, &results, i]() {
            LogEntry localEntry = entry;
            localEntry.message = "Message " + std::to_string(i);
            results[i] = formatter->format(localEntry);
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Verify all results are valid
    for (int i = 0; i < 10; ++i) {
        EXPECT_TRUE(results[i].find("Message " + std::to_string(i)) != std::string::npos);
        EXPECT_TRUE(results[i].find("[INFO]") != std::string::npos);
    }
}