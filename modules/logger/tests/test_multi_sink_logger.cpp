#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "sylife/logger/multi_sink_logger.h"
#include "sylife/logger/i_log_sink.h"
#include <atomic>
#include <vector>
#include <thread>

using namespace sylife::logger;
using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;

// Mock sink for testing
class MockLogSink : public ILogSink {
public:
    MOCK_METHOD(void, write, (const LogEntry& entry), (override));
    MOCK_METHOD(void, flush, (), (override));
    MOCK_METHOD(bool, isActive, (), (const, noexcept, override));
    MOCK_METHOD(void, setActive, (bool active), (noexcept, override));
};

class MultiSinkLoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        logger = std::make_unique<MultiSinkLogger>(LogLevel::Debug);
        mockSink1 = std::make_shared<MockLogSink>();
        mockSink2 = std::make_shared<MockLogSink>();
    }

    std::unique_ptr<MultiSinkLogger> logger;
    std::shared_ptr<MockLogSink> mockSink1;
    std::shared_ptr<MockLogSink> mockSink2;
};

TEST_F(MultiSinkLoggerTest, DefaultConstructor) {
    MultiSinkLogger defaultLogger;
    EXPECT_EQ(defaultLogger.getMinimumLevel(), LogLevel::Info);
    EXPECT_EQ(defaultLogger.getSinkCount(), 0);
}

TEST_F(MultiSinkLoggerTest, ConstructorWithLevel) {
    EXPECT_EQ(logger->getMinimumLevel(), LogLevel::Debug);
    EXPECT_TRUE(logger->isLevelEnabled(LogLevel::Debug));
    EXPECT_TRUE(logger->isLevelEnabled(LogLevel::Critical));
}

TEST_F(MultiSinkLoggerTest, SetGetMinimumLevel) {
    logger->setMinimumLevel(LogLevel::Warning);
    EXPECT_EQ(logger->getMinimumLevel(), LogLevel::Warning);
    
    EXPECT_FALSE(logger->isLevelEnabled(LogLevel::Debug));
    EXPECT_FALSE(logger->isLevelEnabled(LogLevel::Info));
    EXPECT_TRUE(logger->isLevelEnabled(LogLevel::Warning));
    EXPECT_TRUE(logger->isLevelEnabled(LogLevel::Error));
    EXPECT_TRUE(logger->isLevelEnabled(LogLevel::Critical));
}

TEST_F(MultiSinkLoggerTest, AddRemoveSinks) {
    EXPECT_EQ(logger->getSinkCount(), 0);
    
    logger->addSink(mockSink1);
    EXPECT_EQ(logger->getSinkCount(), 1);
    
    logger->addSink(mockSink2);
    EXPECT_EQ(logger->getSinkCount(), 2);
    
    logger->removeSink(mockSink1);
    EXPECT_EQ(logger->getSinkCount(), 1);
    
    logger->clearSinks();
    EXPECT_EQ(logger->getSinkCount(), 0);
}

TEST_F(MultiSinkLoggerTest, AddNullSink) {
    logger->addSink(nullptr);
    EXPECT_EQ(logger->getSinkCount(), 0);
}

TEST_F(MultiSinkLoggerTest, LogToMultipleSinks) {
    ON_CALL(*mockSink1, isActive()).WillByDefault(Return(true));
    ON_CALL(*mockSink2, isActive()).WillByDefault(Return(true));
    
    EXPECT_CALL(*mockSink1, write(_)).Times(1);
    EXPECT_CALL(*mockSink2, write(_)).Times(1);
    
    logger->addSink(mockSink1);
    logger->addSink(mockSink2);
    
    logger->info("Test message");
}

TEST_F(MultiSinkLoggerTest, LogFilteredByLevel) {
    logger->setMinimumLevel(LogLevel::Error);
    
    ON_CALL(*mockSink1, isActive()).WillByDefault(Return(true));
    
    EXPECT_CALL(*mockSink1, write(_)).Times(0);
    
    logger->addSink(mockSink1);
    logger->info("This should be filtered out");
}

TEST_F(MultiSinkLoggerTest, LogNotFilteredByLevel) {
    logger->setMinimumLevel(LogLevel::Info);
    
    ON_CALL(*mockSink1, isActive()).WillByDefault(Return(true));
    
    EXPECT_CALL(*mockSink1, write(_)).Times(1);
    
    logger->addSink(mockSink1);
    logger->error("This should not be filtered");
}

TEST_F(MultiSinkLoggerTest, InactiveSinkNotCalled) {
    ON_CALL(*mockSink1, isActive()).WillByDefault(Return(false));
    
    EXPECT_CALL(*mockSink1, write(_)).Times(0);
    
    logger->addSink(mockSink1);
    logger->info("Test message");
}

TEST_F(MultiSinkLoggerTest, FlushAllSinks) {
    ON_CALL(*mockSink1, isActive()).WillByDefault(Return(true));
    ON_CALL(*mockSink2, isActive()).WillByDefault(Return(true));
    
    EXPECT_CALL(*mockSink1, flush()).Times(1);
    EXPECT_CALL(*mockSink2, flush()).Times(1);
    
    logger->addSink(mockSink1);
    logger->addSink(mockSink2);
    
    logger->flush();
}

TEST_F(MultiSinkLoggerTest, ConvenienceMethods) {
    ON_CALL(*mockSink1, isActive()).WillByDefault(Return(true));
    
    EXPECT_CALL(*mockSink1, write(_)).Times(5);
    
    logger->addSink(mockSink1);
    
    logger->debug("Debug message");
    logger->info("Info message");
    logger->warning("Warning message");
    logger->error("Error message");
    logger->critical("Critical message");
}

TEST_F(MultiSinkLoggerTest, ThreadSafety) {
    ON_CALL(*mockSink1, isActive()).WillByDefault(Return(true));
    
    // We can't easily test the exact number of calls due to threading,
    // but we can verify no crashes occur
    EXPECT_CALL(*mockSink1, write(_)).Times(::testing::AtLeast(100));
    
    logger->addSink(mockSink1);
    
    std::vector<std::thread> threads;
    std::atomic<int> counter{0};
    
    // Create multiple threads that log simultaneously
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([this, &counter]() {
            for (int j = 0; j < 10; ++j) {
                logger->info("Thread message " + std::to_string(counter.fetch_add(1)));
            }
        });
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    EXPECT_EQ(counter.load(), 100);
}

TEST_F(MultiSinkLoggerTest, ExceptionHandling) {
    ON_CALL(*mockSink1, isActive()).WillByDefault(Return(true));
    ON_CALL(*mockSink2, isActive()).WillByDefault(Return(true));
    
    // First sink throws an exception
    EXPECT_CALL(*mockSink1, write(_))
        .WillOnce(::testing::Throw(std::runtime_error("Sink error")));
    
    // Second sink should still be called
    EXPECT_CALL(*mockSink2, write(_)).Times(1);
    
    logger->addSink(mockSink1);
    logger->addSink(mockSink2);
    
    // Should not throw - should continue with other sinks
    EXPECT_NO_THROW(logger->info("Test message"));
}