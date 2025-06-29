#pragma once

#include <string>
#include "i_log_sink.h"

namespace sylife::logger {

/**
 * @brief Interface for log message formatting
 * 
 * Separates formatting logic from logging logic (Single Responsibility)
 */
class ILogFormatter {
public:
    virtual ~ILogFormatter() = default;

    /**
     * @brief Format a log entry into a string
     * @param entry The log entry to format
     * @return Formatted string representation
     */
    virtual std::string format(const LogEntry& entry) const = 0;
};

} // namespace sylife::logger