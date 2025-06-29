#include <gtest/gtest.h>
#include "sylife/logger/console_sink.h"
#include "sylife/logger/default_formatter.h"
#include <sstream>

using namespace sylife::logger;

class ConsoleSinkTest : public ::testing::Test {
protected:
    void SetUp() override {
        formatter = std::make_shared<DefaultFormatter>(false, false);
        sink = std::make_unique<ConsoleSink>(formatter, false);
    }

    std::shared_ptr<DefaultFormatter> formatter;
    std::unique_ptr<ConsoleSink> sink;
};

TEST_F(ConsoleSinkTest, DefaultConstruction) {
    ConsoleSink defaultSink;
    EXPECT_TRUE(defaultSink.isActive());
    EXPECT_TRUE(defaultSink.isColorOutputEnabled());
}

TEST_F(ConsoleSinkTest, IsActiveByDefault) {
    EXPECT_TRUE(sink->isActive());
}

TEST_F(ConsoleSinkTest, SetActiveState) {
    sink->setActive(false);
    EXPECT_FALSE(sink->isActive());
    
    sink->setActive(true);
    EXPECT_TRUE(sink->isActive());
}

TEST_F(ConsoleSinkTest, ColorOutputToggle) {
    sink->setColorOutput(false);
    EXPECT_FALSE(sink->isColorOutputEnabled());
    
    sink->setColorOutput(true);
    EXPECT_TRUE(sink->isColorOutputEnabled());
}

TEST_F(ConsoleSinkTest, WriteWhenActive) {
    LogEntry entry{
        .level = LogLevel::Info,
        .message = "Test message",
        .location = std::source_location::current(),
        .timestamp = std::chrono::system_clock::now(),
        .thread_id = std::this_thread::get_id()
    };
    
    // Should not throw when active
    EXPECT_NO_THROW(sink->write(entry));
}

TEST_F(ConsoleSinkTest, WriteWhenInactive) {
    sink->setActive(false);
    
    LogEntry entry{
        .level = LogLevel::Info,
        .message = "Test message",
        .location = std::source_location::current(),
        .timestamp = std::chrono::system_clock::now(),
        .thread_id = std::this_thread::get_id()
    };
    
    // Should not throw when inactive (just returns early)
    EXPECT_NO_THROW(sink->write(entry));
}

TEST_F(ConsoleSinkTest, FlushWhenActive) {
    EXPECT_NO_THROW(sink->flush());
}

TEST_F(ConsoleSinkTest, FlushWhenInactive) {
    sink->setActive(false);
    EXPECT_NO_THROW(sink->flush());
}