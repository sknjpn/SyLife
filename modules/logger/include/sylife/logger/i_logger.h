#pragma once

#include <string>
#include <string_view>
#include <source_location>

namespace sylife::logger {

/**
 * @brief Log levels for categorizing log messages
 */
enum class LogLevel {
    Debug = 0,
    Info = 1,
    Warning = 2,
    Error = 3,
    Critical = 4
};

/**
 * @brief Convert LogLevel to string representation
 */
constexpr std::string_view logLevelToString(LogLevel level) noexcept {
    switch (level) {
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Info: return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error: return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

/**
 * @brief Logger interface - pure virtual base class
 * 
 * This interface follows SOLID principles:
 * - Single Responsibility: Only responsible for logging
 * - Interface Segregation: Minimal interface with only essential methods
 * - Dependency Inversion: Clients depend on this abstraction, not concrete implementations
 */
class ILogger {
public:
    virtual ~ILogger() = default;

    /**
     * @brief Log a message with the specified level
     * @param level The severity level of the message
     * @param message The message to log
     * @param location Source location information (C++20)
     */
    virtual void log(LogLevel level, 
                    std::string_view message,
                    const std::source_location& location = std::source_location::current()) = 0;

    /**
     * @brief Check if a log level is enabled
     * @param level The level to check
     * @return true if the level is enabled for logging
     */
    virtual bool isLevelEnabled(LogLevel level) const noexcept = 0;

    /**
     * @brief Set the minimum log level
     * @param level Messages below this level will be ignored
     */
    virtual void setMinimumLevel(LogLevel level) noexcept = 0;

    /**
     * @brief Get the current minimum log level
     * @return The current minimum log level
     */
    virtual LogLevel getMinimumLevel() const noexcept = 0;

    /**
     * @brief Flush any buffered log messages
     */
    virtual void flush() = 0;

    // Convenience methods with default implementations
    void debug(std::string_view message, const std::source_location& loc = std::source_location::current()) {
        log(LogLevel::Debug, message, loc);
    }

    void info(std::string_view message, const std::source_location& loc = std::source_location::current()) {
        log(LogLevel::Info, message, loc);
    }

    void warning(std::string_view message, const std::source_location& loc = std::source_location::current()) {
        log(LogLevel::Warning, message, loc);
    }

    void error(std::string_view message, const std::source_location& loc = std::source_location::current()) {
        log(LogLevel::Error, message, loc);
    }

    void critical(std::string_view message, const std::source_location& loc = std::source_location::current()) {
        log(LogLevel::Critical, message, loc);
    }
};

} // namespace sylife::logger