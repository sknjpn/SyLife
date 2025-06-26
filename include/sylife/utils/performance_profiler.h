#pragma once

#include "sylife/core/common.h"
#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <fstream>

namespace sylife::utils {

/**
 * @brief Performance profiling data for a single scope
 */
struct ProfileData {
    std::string name;
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::microseconds totalTime{0};
    std::chrono::microseconds minTime{std::chrono::microseconds::max()};
    std::chrono::microseconds maxTime{0};
    std::chrono::microseconds avgTime{0};
    size_t callCount = 0;
    std::thread::id threadId;
    
    void addSample(std::chrono::microseconds duration) {
        totalTime += duration;
        minTime = std::min(minTime, duration);
        maxTime = std::max(maxTime, duration);
        callCount++;
        avgTime = totalTime / callCount;
    }
    
    void reset() {
        totalTime = std::chrono::microseconds{0};
        minTime = std::chrono::microseconds::max();
        maxTime = std::chrono::microseconds{0};
        avgTime = std::chrono::microseconds{0};
        callCount = 0;
    }
};

/**
 * @brief Performance profiler for measuring execution times
 */
class PerformanceProfiler {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to profiler instance
     */
    static PerformanceProfiler& getInstance();

    /**
     * @brief Enable/disable profiling
     * @param enabled true to enable profiling
     */
    void setEnabled(bool enabled);

    /**
     * @brief Check if profiling is enabled
     * @return true if enabled
     */
    bool isEnabled() const;

    /**
     * @brief Start profiling scope
     * @param name Scope name
     */
    void startScope(const std::string& name);

    /**
     * @brief End profiling scope
     * @param name Scope name
     */
    void endScope(const std::string& name);

    /**
     * @brief Add custom timing sample
     * @param name Scope name
     * @param duration Duration to add
     */
    void addSample(const std::string& name, std::chrono::microseconds duration);

    /**
     * @brief Get profiling data for scope
     * @param name Scope name
     * @return Profile data
     */
    ProfileData getProfileData(const std::string& name) const;

    /**
     * @brief Get all profiling data
     * @return Map of all profile data
     */
    std::unordered_map<std::string, ProfileData> getAllProfileData() const;

    /**
     * @brief Reset all profiling data
     */
    void reset();

    /**
     * @brief Reset specific scope
     * @param name Scope name
     */
    void resetScope(const std::string& name);

    /**
     * @brief Generate profiling report
     * @param sortBy Sort criteria ("name", "total", "avg", "max", "calls")
     * @return Formatted report string
     */
    std::string generateReport(const std::string& sortBy = "total") const;

    /**
     * @brief Save profiling data to file
     * @param filename Output filename
     * @param format Output format ("text", "csv", "json")
     */
    void saveToFile(const std::string& filename, const std::string& format = "text") const;

    /**
     * @brief Set profiling overhead threshold
     * @param threshold Minimum duration to profile (in microseconds)
     */
    void setOverheadThreshold(std::chrono::microseconds threshold);

    /**
     * @brief Enable automatic report generation
     * @param interval Report generation interval in seconds
     * @param filename Output filename template
     */
    void enableAutoReporting(int interval, const std::string& filename = "profile_report_{timestamp}.txt");

    /**
     * @brief Disable automatic report generation
     */
    void disableAutoReporting();

    /**
     * @brief Get profiling overhead statistics
     * @return Overhead statistics
     */
    struct OverheadStats {
        std::chrono::microseconds totalOverhead{0};
        size_t measurementCount = 0;
        std::chrono::microseconds avgOverhead{0};
        double overheadPercentage = 0.0;
    };

    OverheadStats getOverheadStats() const;

private:
    PerformanceProfiler() = default;
    ~PerformanceProfiler();

    void autoReportingTask();
    std::string formatDuration(std::chrono::microseconds duration) const;
    
    mutable std::mutex mutex_;
    std::atomic<bool> enabled_{false};
    std::unordered_map<std::string, ProfileData> profiles_;
    std::unordered_map<std::thread::id, std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point>> activeScopes_;
    
    // Overhead measurement
    std::chrono::microseconds overheadThreshold_{1}; // 1 microsecond
    mutable std::atomic<std::chrono::microseconds> totalOverhead_{std::chrono::microseconds{0}};
    mutable std::atomic<size_t> overheadMeasurements_{0};
    
    // Auto reporting
    std::atomic<bool> autoReportingEnabled_{false};
    std::atomic<int> reportingInterval_{60}; // 60 seconds
    std::string reportingFilename_;
    std::thread autoReportingThread_;
    std::atomic<bool> shutdownRequested_{false};
};

/**
 * @brief RAII profiling scope guard
 */
class ProfileScope {
public:
    explicit ProfileScope(const std::string& name)
        : name_(name) {
        auto start = std::chrono::high_resolution_clock::now();
        PerformanceProfiler::getInstance().startScope(name_);
        auto end = std::chrono::high_resolution_clock::now();
        overhead_ = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    }

    ~ProfileScope() {
        auto start = std::chrono::high_resolution_clock::now();
        PerformanceProfiler::getInstance().endScope(name_);
        auto end = std::chrono::high_resolution_clock::now();
        overhead_ += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // Track profiling overhead
        PerformanceProfiler::getInstance().addSample("_profiler_overhead", overhead_);
    }

    ProfileScope(const ProfileScope&) = delete;
    ProfileScope& operator=(const ProfileScope&) = delete;

private:
    std::string name_;
    std::chrono::microseconds overhead_{0};
};

/**
 * @brief Function-level profiling helper
 */
class FunctionProfiler {
public:
    explicit FunctionProfiler(const std::string& functionName)
        : scope_(functionName) {
    }

private:
    ProfileScope scope_;
};

/**
 * @brief CPU usage monitor
 */
class CPUMonitor {
public:
    /**
     * @brief Start CPU monitoring
     */
    void start();

    /**
     * @brief Stop CPU monitoring
     */
    void stop();

    /**
     * @brief Get current CPU usage percentage
     * @return CPU usage (0.0 to 100.0)
     */
    double getCurrentUsage() const;

    /**
     * @brief Get average CPU usage since start
     * @return Average CPU usage (0.0 to 100.0)
     */
    double getAverageUsage() const;

    /**
     * @brief Get peak CPU usage since start
     * @return Peak CPU usage (0.0 to 100.0)
     */
    double getPeakUsage() const;

    /**
     * @brief Reset CPU usage statistics
     */
    void reset();

private:
    void monitoringTask();
    double calculateCPUUsage();

    std::atomic<bool> monitoring_{false};
    std::atomic<bool> shutdownRequested_{false};
    std::thread monitoringThread_;
    
    mutable std::mutex mutex_;
    std::vector<double> usageHistory_;
    std::atomic<double> currentUsage_{0.0};
    std::atomic<double> peakUsage_{0.0};
    
    // Platform-specific CPU monitoring data
    #ifdef _WIN32
    uint64_t previousIdleTime_ = 0;
    uint64_t previousKernelTime_ = 0;
    uint64_t previousUserTime_ = 0;
    #else
    uint64_t previousIdleTime_ = 0;
    uint64_t previousTotalTime_ = 0;
    #endif
};

/**
 * @brief Memory usage monitor
 */
class MemoryMonitor {
public:
    /**
     * @brief Memory usage statistics
     */
    struct MemoryStats {
        size_t totalPhysical = 0;
        size_t availablePhysical = 0;
        size_t usedPhysical = 0;
        size_t totalVirtual = 0;
        size_t availableVirtual = 0;
        size_t usedVirtual = 0;
        size_t processMemory = 0;
        size_t peakProcessMemory = 0;
        double physicalUsagePercentage = 0.0;
        double virtualUsagePercentage = 0.0;
    };

    /**
     * @brief Get current memory usage statistics
     * @return Memory statistics
     */
    static MemoryStats getCurrentStats();

    /**
     * @brief Start memory monitoring
     */
    void start();

    /**
     * @brief Stop memory monitoring
     */
    void stop();

    /**
     * @brief Get memory usage history
     * @return Vector of memory statistics over time
     */
    std::vector<MemoryStats> getHistory() const;

    /**
     * @brief Get peak memory usage
     * @return Peak memory statistics
     */
    MemoryStats getPeakUsage() const;

    /**
     * @brief Reset memory monitoring data
     */
    void reset();

private:
    void monitoringTask();

    std::atomic<bool> monitoring_{false};
    std::atomic<bool> shutdownRequested_{false};
    std::thread monitoringThread_;
    
    mutable std::mutex mutex_;
    std::vector<MemoryStats> history_;
    MemoryStats peakStats_;
};

/**
 * @brief Frame rate monitor for real-time applications
 */
class FrameRateMonitor {
public:
    /**
     * @brief Record frame completion
     */
    void recordFrame();

    /**
     * @brief Get current FPS
     * @return Frames per second
     */
    double getCurrentFPS() const;

    /**
     * @brief Get average FPS over window
     * @return Average frames per second
     */
    double getAverageFPS() const;

    /**
     * @brief Get minimum FPS in window
     * @return Minimum frames per second
     */
    double getMinFPS() const;

    /**
     * @brief Get maximum FPS in window
     * @return Maximum frames per second
     */
    double getMaxFPS() const;

    /**
     * @brief Get frame time statistics
     */
    struct FrameStats {
        std::chrono::microseconds avgFrameTime{0};
        std::chrono::microseconds minFrameTime{std::chrono::microseconds::max()};
        std::chrono::microseconds maxFrameTime{0};
        std::chrono::microseconds last1PercentFrameTime{0}; // 99th percentile
        double currentFPS = 0.0;
        double avgFPS = 0.0;
        size_t frameCount = 0;
    };

    FrameStats getFrameStats() const;

    /**
     * @brief Reset frame rate monitoring
     */
    void reset();

    /**
     * @brief Set monitoring window size
     * @param windowSize Number of frames to average over
     */
    void setWindowSize(size_t windowSize);

private:
    mutable std::mutex mutex_;
    std::vector<std::chrono::high_resolution_clock::time_point> frameTimes_;
    size_t windowSize_ = 60; // Default to 60 frames
    size_t frameCount_ = 0;
};

// Convenience macros for profiling
#ifdef SYLIFE_DEBUG
    #define SYLIFE_PROFILE_SCOPE(name) \
        sylife::utils::ProfileScope _profile_scope(name)
    
    #define SYLIFE_PROFILE_FUNCTION() \
        sylife::utils::FunctionProfiler _function_profiler(__FUNCTION__)
    
    #define SYLIFE_PROFILE_ENABLED() \
        sylife::utils::PerformanceProfiler::getInstance().isEnabled()
    
    #define SYLIFE_PROFILE_START(name) \
        sylife::utils::PerformanceProfiler::getInstance().startScope(name)
    
    #define SYLIFE_PROFILE_END(name) \
        sylife::utils::PerformanceProfiler::getInstance().endScope(name)
#else
    #define SYLIFE_PROFILE_SCOPE(name) ((void)0)
    #define SYLIFE_PROFILE_FUNCTION() ((void)0)
    #define SYLIFE_PROFILE_ENABLED() false
    #define SYLIFE_PROFILE_START(name) ((void)0)
    #define SYLIFE_PROFILE_END(name) ((void)0)
#endif

} // namespace sylife::utils