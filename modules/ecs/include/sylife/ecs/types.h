#pragma once

#include <cstdint>
#include <array>
#include <bitset>
#include <type_traits>
#include <typeindex>
#include <typeinfo>

namespace sylife::ecs {

/**
 * @brief Type-safe entity identifier
 */
class Entity {
public:
    using IdType = std::uint32_t;
    using VersionType = std::uint32_t;
    
    static constexpr IdType NULL_ID = 0;
    static constexpr VersionType NULL_VERSION = 0;
    
    constexpr Entity() noexcept : id_(NULL_ID), version_(NULL_VERSION) {}
    constexpr Entity(IdType id, VersionType version) noexcept : id_(id), version_(version) {}
    
    constexpr IdType getId() const noexcept { return id_; }
    constexpr VersionType getVersion() const noexcept { return version_; }
    
    constexpr bool isNull() const noexcept { 
        return id_ == NULL_ID && version_ == NULL_VERSION; 
    }
    
    constexpr bool operator==(const Entity& other) const noexcept {
        return id_ == other.id_ && version_ == other.version_;
    }
    
    constexpr bool operator!=(const Entity& other) const noexcept {
        return !(*this == other);
    }
    
    constexpr bool operator<(const Entity& other) const noexcept {
        return id_ < other.id_ || (id_ == other.id_ && version_ < other.version_);
    }

private:
    IdType id_;
    VersionType version_;
};

/**
 * @brief Component type identifier
 */
class ComponentType {
public:
    using IdType = std::uint32_t;
    
    constexpr ComponentType() noexcept : id_(0) {}
    constexpr explicit ComponentType(IdType id) noexcept : id_(id) {}
    
    constexpr IdType getId() const noexcept { return id_; }
    
    constexpr bool operator==(const ComponentType& other) const noexcept {
        return id_ == other.id_;
    }
    
    constexpr bool operator!=(const ComponentType& other) const noexcept {
        return id_ != other.id_;
    }
    
    constexpr bool operator<(const ComponentType& other) const noexcept {
        return id_ < other.id_;
    }

private:
    IdType id_;
};

/**
 * @brief Maximum number of component types
 */
static constexpr std::size_t MAX_COMPONENTS = 64;

/**
 * @brief Component signature using bitset for efficient operations
 */
using ComponentSignature = std::bitset<MAX_COMPONENTS>;

/**
 * @brief Component type registry for type-safe component management
 */
class ComponentTypeRegistry {
public:
    template<typename T>
    static ComponentType getComponentType() {
        static_assert(std::is_base_of_v<ComponentBase, T>, 
                     "T must inherit from ComponentBase");
        
        static const ComponentType type = generateComponentType<T>();
        return type;
    }
    
    template<typename T>
    static constexpr std::size_t getComponentIndex() {
        return getComponentType<T>().getId();
    }

private:
    template<typename T>
    static ComponentType generateComponentType() {
        static ComponentType::IdType next_id = 0;
        return ComponentType(next_id++);
    }
};

/**
 * @brief Base class for all components
 */
struct ComponentBase {
    virtual ~ComponentBase() = default;
    
    /**
     * @brief Clone the component
     * @return Unique pointer to cloned component
     */
    virtual std::unique_ptr<ComponentBase> clone() const = 0;
    
    /**
     * @brief Get the component type
     * @return Component type identifier
     */
    virtual ComponentType getType() const = 0;
    
    /**
     * @brief Get the component type name
     * @return Component type name
     */
    virtual const char* getTypeName() const = 0;
};

/**
 * @brief CRTP base for typed components
 */
template<typename T>
struct Component : public ComponentBase {
    static_assert(std::is_final_v<T>, "Component types must be final");
    
    ComponentType getType() const override {
        return ComponentTypeRegistry::getComponentType<T>();
    }
    
    const char* getTypeName() const override {
        return typeid(T).name();
    }
    
    std::unique_ptr<ComponentBase> clone() const override {
        return std::make_unique<T>(static_cast<const T&>(*this));
    }
};

/**
 * @brief System priority for execution order
 */
enum class SystemPriority : std::int32_t {
    Highest = -1000,
    High = -100,
    Normal = 0,
    Low = 100,
    Lowest = 1000
};

/**
 * @brief System execution phase
 */
enum class SystemPhase {
    PreUpdate,
    Update,
    PostUpdate,
    Render,
    Cleanup
};

/**
 * @brief Performance profiling data
 */
struct ProfileData {
    double execution_time_ms = 0.0;
    std::size_t entities_processed = 0;
    std::size_t memory_usage_bytes = 0;
};

/**
 * @brief Event type identifier
 */
using EventType = std::type_index;

/**
 * @brief Event priority for processing order
 */
enum class EventPriority : std::int32_t {
    Immediate = -1000,
    High = -100,
    Normal = 0,
    Low = 100,
    Deferred = 1000
};

} // namespace sylife::ecs

// Hash specializations for unordered containers
namespace std {
    template<>
    struct hash<sylife::ecs::Entity> {
        std::size_t operator()(const sylife::ecs::Entity& entity) const noexcept {
            return std::hash<std::uint64_t>{}(
                (static_cast<std::uint64_t>(entity.getId()) << 32) |
                static_cast<std::uint64_t>(entity.getVersion())
            );
        }
    };
    
    template<>
    struct hash<sylife::ecs::ComponentType> {
        std::size_t operator()(const sylife::ecs::ComponentType& type) const noexcept {
            return std::hash<std::uint32_t>{}(type.getId());
        }
    };
}