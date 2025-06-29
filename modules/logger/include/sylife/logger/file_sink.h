#pragma once

#include "i_log_sink.h"
#include "i_log_formatter.h"
#include <memory>
#include <mutex>
#include <fstream>
#include <filesystem>

namespace sylife::logger {

/**
 * @brief Log sink that outputs to a file with rotation support
 */
class FileSink : public ILogSink {
public:
    struct Config {
        std::filesystem::path filePath;
        size_t maxFileSize = 10 * 1024 * 1024; // 10MB default
        size_t maxFiles = 5;                    // Keep 5 rotated files
        bool appendMode = true;
    };

    explicit FileSink(const Config& config,
                     std::shared_ptr<ILogFormatter> formatter = nullptr);
    
    ~FileSink() override;

    // ILogSink interface
    void write(const LogEntry& entry) override;
    void flush() override;
    bool isActive() const noexcept override;
    void setActive(bool active) noexcept override;

private:
    mutable std::mutex m_mutex;
    Config m_config;
    std::shared_ptr<ILogFormatter> m_formatter;
    std::ofstream m_file;
    std::atomic<bool> m_active{true};
    std::atomic<size_t> m_currentSize{0};

    void openFile();
    void rotateFiles();
    void checkRotation();
};

} // namespace sylife::logger