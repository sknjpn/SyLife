#include "sylife/logger/multi_sink_logger.h"
#include "sylife/logger/console_sink.h"
#include "sylife/logger/file_sink.h"
#include "sylife/logger/default_formatter.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <filesystem>

using namespace sylife::logger;

void benchmarkLogging(const std::string& testName, 
                     std::shared_ptr<ILogger> logger, 
                     int messageCount) {
    std::cout << "Running benchmark: " << testName << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < messageCount; ++i) {
        logger->info("Benchmark message " + std::to_string(i) + 
                    " - this is a test message for performance measurement");
    }
    
    logger->flush();
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double messagesPerSecond = (messageCount * 1000000.0) / duration.count();
    
    std::cout << "  Messages: " << messageCount << std::endl;
    std::cout << "  Duration: " << duration.count() << " microseconds" << std::endl;
    std::cout << "  Rate: " << static_cast<int>(messagesPerSecond) << " messages/second" << std::endl;
    std::cout << "  Avg: " << (duration.count() / messageCount) << " microseconds/message" << std::endl;
    std::cout << std::endl;
}

void benchmarkMultithreaded(std::shared_ptr<ILogger> logger, 
                           int threadCount, 
                           int messagesPerThread) {
    std::cout << "Running multithreaded benchmark:" << std::endl;
    std::cout << "  Threads: " << threadCount << std::endl;
    std::cout << "  Messages per thread: " << messagesPerThread << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<std::thread> threads;
    for (int t = 0; t < threadCount; ++t) {
        threads.emplace_back([logger, t, messagesPerThread]() {
            for (int i = 0; i < messagesPerThread; ++i) {
                logger->info("Thread " + std::to_string(t) + 
                           " message " + std::to_string(i));
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    logger->flush();
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    int totalMessages = threadCount * messagesPerThread;
    double messagesPerSecond = (totalMessages * 1000000.0) / duration.count();
    
    std::cout << "  Total messages: " << totalMessages << std::endl;
    std::cout << "  Duration: " << duration.count() << " microseconds" << std::endl;
    std::cout << "  Rate: " << static_cast<int>(messagesPerSecond) << " messages/second" << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "SyLife Logger Performance Test" << std::endl;
    std::cout << "=============================" << std::endl;
    std::cout << std::endl;
    
    // Create test directory
    std::filesystem::create_directories("benchmark_logs");
    
    // Test 1: Console only
    {
        auto logger = std::make_shared<MultiSinkLogger>(LogLevel::Info);
        auto consoleSink = std::make_shared<ConsoleSink>();
        consoleSink->setColorOutput(false);  // Disable colors for cleaner output
        logger->addSink(consoleSink);
        
        benchmarkLogging("Console Only", logger, 1000);
    }
    
    // Test 2: File only
    {
        auto logger = std::make_shared<MultiSinkLogger>(LogLevel::Info);
        
        FileSink::Config config;
        config.filePath = "benchmark_logs/file_only.log";
        config.maxFileSize = 100 * 1024 * 1024;  // 100MB
        auto fileSink = std::make_shared<FileSink>(config);
        
        logger->addSink(fileSink);
        benchmarkLogging("File Only", logger, 10000);
    }
    
    // Test 3: Console + File
    {
        auto logger = std::make_shared<MultiSinkLogger>(LogLevel::Info);
        
        auto consoleSink = std::make_shared<ConsoleSink>();
        consoleSink->setColorOutput(false);
        logger->addSink(consoleSink);
        
        FileSink::Config config;
        config.filePath = "benchmark_logs/console_and_file.log";
        config.maxFileSize = 100 * 1024 * 1024;
        auto fileSink = std::make_shared<FileSink>(config);
        logger->addSink(fileSink);
        
        benchmarkLogging("Console + File", logger, 5000);
    }
    
    // Test 4: Different log levels
    {
        auto logger = std::make_shared<MultiSinkLogger>(LogLevel::Debug);
        
        FileSink::Config config;
        config.filePath = "benchmark_logs/level_test.log";
        auto fileSink = std::make_shared<FileSink>(config);
        logger->addSink(fileSink);
        
        std::cout << "Testing different log levels:" << std::endl;
        
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; ++i) {
            logger->debug("Debug message " + std::to_string(i));
            logger->info("Info message " + std::to_string(i));
            logger->warning("Warning message " + std::to_string(i));
            logger->error("Error message " + std::to_string(i));
        }
        logger->flush();
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "  4000 messages (mixed levels): " << duration.count() << " microseconds" << std::endl;
        std::cout << std::endl;
    }
    
    // Test 5: Multithreaded
    {
        auto logger = std::make_shared<MultiSinkLogger>(LogLevel::Info);
        
        FileSink::Config config;
        config.filePath = "benchmark_logs/multithreaded.log";
        auto fileSink = std::make_shared<FileSink>(config);
        logger->addSink(fileSink);
        
        benchmarkMultithreaded(logger, 4, 1000);
    }
    
    // Test 6: Level filtering performance
    {
        auto logger = std::make_shared<MultiSinkLogger>(LogLevel::Error);
        
        FileSink::Config config;
        config.filePath = "benchmark_logs/filtered.log";
        auto fileSink = std::make_shared<FileSink>(config);
        logger->addSink(fileSink);
        
        std::cout << "Testing level filtering (Debug messages to Error-level logger):" << std::endl;
        
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 10000; ++i) {
            logger->debug("This message should be filtered out");
        }
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "  10000 filtered messages: " << duration.count() << " microseconds" << std::endl;
        std::cout << "  Rate: " << static_cast<int>((10000 * 1000000.0) / duration.count()) << " messages/second" << std::endl;
        std::cout << std::endl;
    }
    
    std::cout << "Performance testing completed!" << std::endl;
    std::cout << "Log files created in benchmark_logs/ directory" << std::endl;
    
    return 0;
}