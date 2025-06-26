#pragma once

/**
 * @file common.h
 * @brief Common definitions and includes for SyLife
 */

#include <Siv3D.hpp>
#include <memory>
#include <vector>
#include <string>
#include <array>
#include <unordered_map>
#include <functional>
#include <type_traits>
#include <concepts>

namespace sylife {

// Forward declarations
class World;
class Asset;
class CellAsset;
class PartAsset;
class ProteinAsset;
class CellState;
class EggState;
class PartState;
class TileState;

// Common constants
namespace constants {
    constexpr double DELTA_TIME = 1.0 / 60.0;
    constexpr double TILE_LENGTH = 100.0;
    constexpr double PHYSICS_SCALE = 1.0;
    constexpr int DEFAULT_WORLD_WIDTH = 100;
    constexpr int DEFAULT_WORLD_HEIGHT = 100;
}

// Version information
namespace version {
    constexpr int MAJOR = 1;
    constexpr int MINOR = 0;
    constexpr int PATCH = 0;
    constexpr const char* STRING = "1.0.0";
}

// Platform detection
#if defined(SYLIFE_DESKTOP)
    #define SYLIFE_PLATFORM_DESKTOP 1
#elif defined(SYLIFE_WEB)
    #define SYLIFE_PLATFORM_WEB 1
#else
    #error "Unknown platform"
#endif

// Build configuration
#if defined(SYLIFE_DEBUG)
    #define SYLIFE_BUILD_DEBUG 1
    #define SYLIFE_ASSERT(condition, message) \
        do { \
            if (!(condition)) { \
                s3d::Print << U"Assertion failed: " << U#condition << U" - " << message; \
                std::abort(); \
            } \
        } while(0)
#else
    #define SYLIFE_BUILD_RELEASE 1
    #define SYLIFE_ASSERT(condition, message) ((void)0)
#endif

// Logging macros
#define SYLIFE_LOG_INFO(message) s3d::Print << U"[INFO] " << message
#define SYLIFE_LOG_WARNING(message) s3d::Print << U"[WARNING] " << message
#define SYLIFE_LOG_ERROR(message) s3d::Print << U"[ERROR] " << message

#if defined(SYLIFE_DEBUG)
    #define SYLIFE_LOG_DEBUG(message) s3d::Print << U"[DEBUG] " << message
#else
    #define SYLIFE_LOG_DEBUG(message) ((void)0)
#endif

// Common type aliases
using UniqueId = s3d::uint64;
using TimeStamp = s3d::uint64;
using EntityId = s3d::uint32;

// Utility concepts
template<typename T>
concept Serializable = requires(T t, s3d::JSON& json) {
    t.save(json);
    t.load(json);
};

template<typename T>
concept Updatable = requires(T t, double deltaTime) {
    t.update(deltaTime);
};

template<typename T>
concept Drawable = requires(T t) {
    t.draw();
};

// Memory management helpers
template<typename T, typename... Args>
std::unique_ptr<T> make_unique_sylife(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
std::shared_ptr<T> make_shared_sylife(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

// Error handling
class SyLifeException : public std::exception {
public:
    explicit SyLifeException(const std::string& message) : message_(message) {}
    const char* what() const noexcept override { return message_.c_str(); }

private:
    std::string message_;
};

#define SYLIFE_THROW(message) throw SyLifeException(message)

// Performance profiling
#if defined(SYLIFE_DEBUG)
    #define SYLIFE_PROFILE_SCOPE(name) \
        static_assert(true) // TODO: Implement profiler
#else
    #define SYLIFE_PROFILE_SCOPE(name) ((void)0)
#endif

} // namespace sylife