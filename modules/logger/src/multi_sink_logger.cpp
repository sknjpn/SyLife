#include "sylife/logger/multi_sink_logger.h"
#include <algorithm>
#include <chrono>
#include <thread>

namespace sylife::logger {

MultiSinkLogger::MultiSinkLogger(LogLevel minimumLevel)
    : m_minimumLevel(minimumLevel) {
}

void MultiSinkLogger::log(LogLevel level, 
                         std::string_view message,
                         const std::source_location& location) {
    if (!isLevelEnabled(level)) {
        return;
    }

    LogEntry entry{
        .level = level,
        .message = message,
        .location = location,
        .timestamp = std::chrono::system_clock::now(),
        .thread_id = std::this_thread::get_id()
    };

    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& sink : m_sinks) {
        if (sink && sink->isActive()) {
            try {
                sink->write(entry);
            } catch (...) {
                // Continue with other sinks even if one fails
                // In a production system, you might want to log this failure
                // to a fallback sink or disable the failing sink
            }
        }
    }
}

bool MultiSinkLogger::isLevelEnabled(LogLevel level) const noexcept {
    return level >= m_minimumLevel.load(std::memory_order_relaxed);
}

void MultiSinkLogger::setMinimumLevel(LogLevel level) noexcept {
    m_minimumLevel.store(level, std::memory_order_relaxed);
}

LogLevel MultiSinkLogger::getMinimumLevel() const noexcept {
    return m_minimumLevel.load(std::memory_order_relaxed);
}

void MultiSinkLogger::flush() {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& sink : m_sinks) {
        if (sink && sink->isActive()) {
            try {
                sink->flush();
            } catch (...) {
                // Continue with other sinks
            }
        }
    }
}

void MultiSinkLogger::addSink(std::shared_ptr<ILogSink> sink) {
    if (!sink) {
        return;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    m_sinks.push_back(std::move(sink));
}

void MultiSinkLogger::removeSink(const std::shared_ptr<ILogSink>& sink) {
    if (!sink) {
        return;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    m_sinks.erase(
        std::remove_if(m_sinks.begin(), m_sinks.end(),
                      [&sink](const std::weak_ptr<ILogSink>& weak_sink) {
                          return weak_sink.lock() == sink;
                      }),
        m_sinks.end()
    );
}

void MultiSinkLogger::clearSinks() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_sinks.clear();
}

size_t MultiSinkLogger::getSinkCount() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_sinks.size();
}

} // namespace sylife::logger