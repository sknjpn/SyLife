#pragma once

#include "i_log_formatter.h"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>

namespace sylife::logger {

/**
 * @brief Default log formatter implementation
 * 
 * Produces output in the format:
 * [2024-06-29 12:34:56.789] [INFO] [main.cpp:42] Message content
 */
class DefaultFormatter : public ILogFormatter {
public:
    DefaultFormatter(bool includeThreadId = false, bool includeLocation = true);
    ~DefaultFormatter() override = default;

    std::string format(const LogEntry& entry) const override;

    // Configuration
    void setIncludeThreadId(bool include) noexcept;
    void setIncludeLocation(bool include) noexcept;
    void setDateTimeFormat(const std::string& format);

private:
    std::atomic<bool> m_includeThreadId;
    std::atomic<bool> m_includeLocation;
    std::string m_dateTimeFormat;

    std::string formatTimestamp(const std::chrono::system_clock::time_point& timestamp) const;
    std::string formatLocation(const std::source_location& location) const;
};

} // namespace sylife::logger