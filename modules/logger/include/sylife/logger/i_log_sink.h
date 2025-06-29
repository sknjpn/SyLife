#pragma once

#include <string_view>
#include <source_location>
#include <chrono>
#include <thread>
#include "i_logger.h"

namespace sylife::logger {

/**
 * @brief Log entry structure containing all log information
 */
struct LogEntry {
    LogLevel level;
    std::string_view message;
    std::source_location location;
    std::chrono::system_clock::time_point timestamp;
    std::thread::id thread_id;
};

/**
 * @brief Interface for log sinks (output destinations)
 * 
 * This allows for multiple output destinations (console, file, network, etc.)
 * following the Open/Closed principle - open for extension, closed for modification
 */
class ILogSink {
public:
    virtual ~ILogSink() = default;

    /**
     * @brief Write a log entry to this sink
     * @param entry The log entry to write
     */
    virtual void write(const LogEntry& entry) = 0;

    /**
     * @brief Flush any buffered data
     */
    virtual void flush() = 0;

    /**
     * @brief Check if this sink is currently active
     * @return true if the sink is active and can receive log entries
     */
    virtual bool isActive() const noexcept = 0;

    /**
     * @brief Set whether this sink is active
     * @param active true to activate, false to deactivate
     */
    virtual void setActive(bool active) noexcept = 0;
};

} // namespace sylife::logger