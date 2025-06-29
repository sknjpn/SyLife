#include "sylife/logger/console_sink.h"
#include "sylife/logger/default_formatter.h"

namespace sylife::logger {

ConsoleSink::ConsoleSink(std::shared_ptr<ILogFormatter> formatter, bool useStderr)
    : m_formatter(formatter ? formatter : std::make_shared<DefaultFormatter>())
    , m_useStderr(useStderr) {
}

void ConsoleSink::write(const LogEntry& entry) {
    if (!isActive()) {
        return;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    
    auto& stream = getOutputStream(entry.level);
    
    if (m_colorOutput.load(std::memory_order_relaxed)) {
        stream << getColorCode(entry.level);
    }
    
    stream << m_formatter->format(entry);
    
    if (m_colorOutput.load(std::memory_order_relaxed)) {
        stream << "\033[0m"; // Reset color
    }
    
    stream << std::endl;
}

void ConsoleSink::flush() {
    if (!isActive()) {
        return;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    std::cout.flush();
    std::cerr.flush();
}

bool ConsoleSink::isActive() const noexcept {
    return m_active.load(std::memory_order_relaxed);
}

void ConsoleSink::setActive(bool active) noexcept {
    m_active.store(active, std::memory_order_relaxed);
}

void ConsoleSink::setColorOutput(bool enabled) noexcept {
    m_colorOutput.store(enabled, std::memory_order_relaxed);
}

bool ConsoleSink::isColorOutputEnabled() const noexcept {
    return m_colorOutput.load(std::memory_order_relaxed);
}

std::ostream& ConsoleSink::getOutputStream(LogLevel level) const noexcept {
    if (m_useStderr.load(std::memory_order_relaxed) && 
        (level >= LogLevel::Warning)) {
        return std::cerr;
    }
    return std::cout;
}

std::string ConsoleSink::getColorCode(LogLevel level) const noexcept {
    switch (level) {
        case LogLevel::Debug:    return "\033[36m"; // Cyan
        case LogLevel::Info:     return "\033[32m"; // Green
        case LogLevel::Warning:  return "\033[33m"; // Yellow
        case LogLevel::Error:    return "\033[31m"; // Red
        case LogLevel::Critical: return "\033[1;31m"; // Bold Red
        default:                 return "";
    }
}

} // namespace sylife::logger