#include "sylife/logger/file_sink.h"
#include "sylife/logger/default_formatter.h"
#include <sstream>
#include <iomanip>

namespace sylife::logger {

FileSink::FileSink(const Config& config, std::shared_ptr<ILogFormatter> formatter)
    : m_config(config)
    , m_formatter(formatter ? formatter : std::make_shared<DefaultFormatter>()) {
    
    // Create directory if it doesn't exist
    if (auto parent = m_config.filePath.parent_path(); !parent.empty()) {
        std::filesystem::create_directories(parent);
    }
    
    openFile();
}

FileSink::~FileSink() {
    if (m_file.is_open()) {
        m_file.close();
    }
}

void FileSink::write(const LogEntry& entry) {
    if (!isActive()) {
        return;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_file.is_open()) {
        openFile();
        if (!m_file.is_open()) {
            return; // Failed to open file
        }
    }

    const auto formatted = m_formatter->format(entry);
    m_file << formatted << std::endl;
    
    // Update current size
    m_currentSize.fetch_add(formatted.size() + 1, std::memory_order_relaxed);
    
    checkRotation();
}

void FileSink::flush() {
    if (!isActive()) {
        return;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_file.is_open()) {
        m_file.flush();
    }
}

bool FileSink::isActive() const noexcept {
    return m_active.load(std::memory_order_relaxed);
}

void FileSink::setActive(bool active) noexcept {
    m_active.store(active, std::memory_order_relaxed);
}

void FileSink::openFile() {
    if (m_file.is_open()) {
        m_file.close();
    }

    std::ios_base::openmode mode = std::ios_base::out;
    if (m_config.appendMode) {
        mode |= std::ios_base::app;
    }

    m_file.open(m_config.filePath, mode);
    
    if (m_file.is_open()) {
        // Get current file size
        m_file.seekp(0, std::ios_base::end);
        m_currentSize.store(static_cast<size_t>(m_file.tellp()), std::memory_order_relaxed);
    }
}

void FileSink::rotateFiles() {
    if (m_file.is_open()) {
        m_file.close();
    }

    // Rotate existing files
    for (size_t i = m_config.maxFiles - 1; i > 0; --i) {
        auto oldFile = m_config.filePath;
        oldFile += "." + std::to_string(i);
        
        auto newFile = m_config.filePath;
        newFile += "." + std::to_string(i + 1);
        
        if (std::filesystem::exists(oldFile)) {
            std::filesystem::rename(oldFile, newFile);
        }
    }

    // Move current file to .1
    if (std::filesystem::exists(m_config.filePath)) {
        auto rotatedFile = m_config.filePath;
        rotatedFile += ".1";
        std::filesystem::rename(m_config.filePath, rotatedFile);
    }

    // Remove excess files
    for (size_t i = m_config.maxFiles + 1; i <= m_config.maxFiles + 10; ++i) {
        auto excessFile = m_config.filePath;
        excessFile += "." + std::to_string(i);
        
        if (std::filesystem::exists(excessFile)) {
            std::filesystem::remove(excessFile);
        }
    }

    // Open new file
    openFile();
}

void FileSink::checkRotation() {
    if (m_currentSize.load(std::memory_order_relaxed) >= m_config.maxFileSize) {
        rotateFiles();
    }
}

} // namespace sylife::logger