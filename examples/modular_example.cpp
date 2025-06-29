#include "sylife/logger/multi_sink_logger.h"
#include "sylife/logger/console_sink.h"
#include "sylife/logger/file_sink.h"
#include "sylife/logger/default_formatter.h"
#include "sylife/di/container.h"
#include <iostream>
#include <memory>

using namespace sylife::logger;
using namespace sylife::di;

/**
 * @brief Example service that depends on logging
 */
class EcosystemSimulator {
public:
    explicit EcosystemSimulator(std::shared_ptr<ILogger> logger)
        : m_logger(std::move(logger)) {
        m_logger->info("EcosystemSimulator initialized");
    }

    void runSimulation() {
        m_logger->info("Starting ecosystem simulation...");
        
        // Simulate some work
        for (int step = 1; step <= 5; ++step) {
            m_logger->debug("Simulation step " + std::to_string(step));
            
            if (step == 3) {
                m_logger->warning("Unusual organism behavior detected at step 3");
            }
            
            // Simulate some processing time
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        m_logger->info("Ecosystem simulation completed successfully");
    }

    void handleError() {
        m_logger->error("Critical error in ecosystem simulation!");
        m_logger->critical("System stability compromised - immediate attention required");
    }

private:
    std::shared_ptr<ILogger> m_logger;
};

/**
 * @brief Factory function for creating configured logger
 */
std::shared_ptr<ILogger> createLogger() {
    // Create formatter with custom settings
    auto formatter = std::make_shared<DefaultFormatter>(
        true,  // Include thread ID
        true   // Include source location
    );
    
    // Create console sink with colors
    auto consoleSink = std::make_shared<ConsoleSink>(formatter, true);
    consoleSink->setColorOutput(true);
    
    // Create file sink with rotation
    FileSink::Config fileConfig;
    fileConfig.filePath = "logs/sylife_example.log";
    fileConfig.maxFileSize = 1024 * 1024; // 1MB
    fileConfig.maxFiles = 5;
    fileConfig.appendMode = true;
    
    auto fileSink = std::make_shared<FileSink>(fileConfig, formatter);
    
    // Create multi-sink logger
    auto logger = std::make_shared<MultiSinkLogger>(LogLevel::Debug);
    logger->addSink(consoleSink);
    logger->addSink(fileSink);
    
    return logger;
}

/**
 * @brief Configure dependency injection container
 */
void configureDI(Container& container) {
    // Register logger as singleton
    container.registerFactory<ILogger>(createLogger, Container::Lifetime::Singleton);
    
    // Register EcosystemSimulator as transient (new instance each time)
    container.registerFactory<EcosystemSimulator>(
        [&container]() {
            auto logger = container.resolve<ILogger>();
            return std::make_shared<EcosystemSimulator>(logger);
        },
        Container::Lifetime::Transient
    );
}

int main() {
    try {
        std::cout << "SyLife Modular Architecture Example\n";
        std::cout << "===================================\n\n";
        
        // Create and configure DI container
        Container container;
        configureDI(container);
        
        // Resolve logger directly to show manual usage
        auto logger = container.resolve<ILogger>();
        logger->info("Application starting...");
        
        // Demonstrate different log levels
        logger->debug("This is a debug message - detailed information for developers");
        logger->info("This is an info message - general application flow");
        logger->warning("This is a warning message - something unusual but not critical");
        
        // Resolve ecosystem simulator (depends on logger via DI)
        auto simulator1 = container.resolve<EcosystemSimulator>();
        simulator1->runSimulation();
        
        // Show that transient lifetime creates new instances
        auto simulator2 = container.resolve<EcosystemSimulator>();
        simulator2->runSimulation();
        
        // Demonstrate error handling
        simulator1->handleError();
        
        // Show that logger is singleton (same instance)
        auto logger2 = container.resolve<ILogger>();
        logger2->info("This message comes from the same logger instance");
        
        // Verify singleton behavior
        if (logger == logger2) {
            logger->info("✓ Logger singleton pattern working correctly");
        } else {
            logger->error("✗ Logger singleton pattern failed!");
        }
        
        logger->info("Application shutting down...");
        logger->flush();
        
        std::cout << "\n✓ Example completed successfully!\n";
        std::cout << "Check 'logs/sylife_example.log' for file output.\n";
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}