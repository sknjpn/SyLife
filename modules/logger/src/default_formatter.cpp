#include "sylife/logger/default_formatter.h"
#include <chrono>
#include <sstream>
#include <iomanip>

namespace sylife::logger {

DefaultFormatter::DefaultFormatter(bool includeThreadId, bool includeLocation)
    : m_includeThreadId(includeThreadId)
    , m_includeLocation(includeLocation)
    , m_dateTimeFormat("%Y-%m-%d %H:%M:%S") {
}

std::string DefaultFormatter::format(const LogEntry& entry) const {
    std::ostringstream oss;
    
    // Timestamp
    oss << "[" << formatTimestamp(entry.timestamp) << "]";
    
    // Log level
    oss << " [" << logLevelToString(entry.level) << "]";
    
    // Thread ID (if enabled)
    if (m_includeThreadId.load(std::memory_order_relaxed)) {
        oss << " [" << entry.thread_id << "]";
    }
    
    // Source location (if enabled)
    if (m_includeLocation.load(std::memory_order_relaxed)) {
        oss << " [" << formatLocation(entry.location) << "]";
    }
    
    // Message
    oss << " " << entry.message;
    
    return oss.str();
}

void DefaultFormatter::setIncludeThreadId(bool include) noexcept {
    m_includeThreadId.store(include, std::memory_order_relaxed);
}

void DefaultFormatter::setIncludeLocation(bool include) noexcept {
    m_includeLocation.store(include, std::memory_order_relaxed);
}

void DefaultFormatter::setDateTimeFormat(const std::string& format) {
    m_dateTimeFormat = format;
}

std::string DefaultFormatter::formatTimestamp(const std::chrono::system_clock::time_point& timestamp) const {
    auto time_t = std::chrono::system_clock::to_time_t(timestamp);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        timestamp.time_since_epoch()) % 1000;
    
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t), m_dateTimeFormat.c_str());
    oss << "." << std::setfill('0') << std::setw(3) << ms.count();
    
    return oss.str();
}

std::string DefaultFormatter::formatLocation(const std::source_location& location) const {
    std::ostringstream oss;
    
    // Extract just the filename from the full path
    std::string filename = location.file_name();
    if (auto pos = filename.find_last_of("/\\"); pos != std::string::npos) {
        filename = filename.substr(pos + 1);
    }
    
    oss << filename << ":" << location.line();
    
    return oss.str();
}

} // namespace sylife::logger