#include <gtest/gtest.h>
#include "sylife/logger/i_logger.h"

using namespace sylife::logger;

TEST(LogLevelTest, LogLevelToString) {
    EXPECT_EQ(logLevelToString(LogLevel::Debug), "DEBUG");
    EXPECT_EQ(logLevelToString(LogLevel::Info), "INFO");
    EXPECT_EQ(logLevelToString(LogLevel::Warning), "WARNING");
    EXPECT_EQ(logLevelToString(LogLevel::Error), "ERROR");
    EXPECT_EQ(logLevelToString(LogLevel::Critical), "CRITICAL");
}

TEST(LogLevelTest, LogLevelComparison) {
    EXPECT_LT(LogLevel::Debug, LogLevel::Info);
    EXPECT_LT(LogLevel::Info, LogLevel::Warning);
    EXPECT_LT(LogLevel::Warning, LogLevel::Error);
    EXPECT_LT(LogLevel::Error, LogLevel::Critical);
}

TEST(LogLevelTest, LogLevelValues) {
    EXPECT_EQ(static_cast<int>(LogLevel::Debug), 0);
    EXPECT_EQ(static_cast<int>(LogLevel::Info), 1);
    EXPECT_EQ(static_cast<int>(LogLevel::Warning), 2);
    EXPECT_EQ(static_cast<int>(LogLevel::Error), 3);
    EXPECT_EQ(static_cast<int>(LogLevel::Critical), 4);
}