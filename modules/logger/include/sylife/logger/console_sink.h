#pragma once

#include "i_log_sink.h"
#include "i_log_formatter.h"
#include <memory>
#include <mutex>
#include <iostream>

namespace sylife::logger {

/**
 * @brief Log sink that outputs to console (stdout/stderr)
 */
class ConsoleSink : public ILogSink {
public:
    /**
     * @brief Construct with optional custom formatter
     * @param formatter Custom formatter (uses default if nullptr)
     * @param useStderr If true, errors go to stderr, others to stdout
     */
    explicit ConsoleSink(std::shared_ptr<ILogFormatter> formatter = nullptr,
                        bool useStderr = true);
    
    ~ConsoleSink() override = default;

    // ILogSink interface
    void write(const LogEntry& entry) override;
    void flush() override;
    bool isActive() const noexcept override;
    void setActive(bool active) noexcept override;

    // Console-specific features
    void setColorOutput(bool enabled) noexcept;
    bool isColorOutputEnabled() const noexcept;

private:
    mutable std::mutex m_mutex;
    std::shared_ptr<ILogFormatter> m_formatter;
    std::atomic<bool> m_active{true};
    std::atomic<bool> m_useStderr;
    std::atomic<bool> m_colorOutput{true};

    std::ostream& getOutputStream(LogLevel level) const noexcept;
    std::string getColorCode(LogLevel level) const noexcept;
};

} // namespace sylife::logger