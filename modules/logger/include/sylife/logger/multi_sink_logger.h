#pragma once

#include "i_logger.h"
#include "i_log_sink.h"
#include <vector>
#include <memory>
#include <mutex>
#include <atomic>

namespace sylife::logger {

/**
 * @brief Concrete logger implementation that supports multiple sinks
 * 
 * Thread-safe implementation that can write to multiple destinations
 */
class MultiSinkLogger : public ILogger {
public:
    explicit MultiSinkLogger(LogLevel minimumLevel = LogLevel::Info);
    ~MultiSinkLogger() override = default;

    // ILogger interface implementation
    void log(LogLevel level, 
            std::string_view message,
            const std::source_location& location = std::source_location::current()) override;
    
    bool isLevelEnabled(LogLevel level) const noexcept override;
    void setMinimumLevel(LogLevel level) noexcept override;
    LogLevel getMinimumLevel() const noexcept override;
    void flush() override;

    // Sink management
    void addSink(std::shared_ptr<ILogSink> sink);
    void removeSink(const std::shared_ptr<ILogSink>& sink);
    void clearSinks();
    size_t getSinkCount() const;

private:
    mutable std::mutex m_mutex;
    std::vector<std::shared_ptr<ILogSink>> m_sinks;
    std::atomic<LogLevel> m_minimumLevel;
};

} // namespace sylife::logger