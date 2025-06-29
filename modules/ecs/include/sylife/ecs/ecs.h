#pragma once

/**
 * @file ecs.h
 * @brief Main header for the SyLife Entity Component System
 * 
 * This header provides a complete ECS framework with:
 * - Type-safe entities and components
 * - Cache-friendly component storage
 * - System lifecycle management
 * - Event-driven communication
 * - Performance profiling
 * - Thread safety
 * 
 * Usage example:
 * @code
 * #include <sylife/ecs/ecs.h>
 * 
 * // Define components
 * struct Position final : public sylife::ecs::Component<Position> {
 *     float x = 0.0f, y = 0.0f;
 * };
 * 
 * // Define systems
 * class MovementSystem : public sylife::ecs::ComponentSystem<MovementSystem, Position> {
 * public:
 *     MovementSystem() : ComponentSystem("MovementSystem") {}
 *     
 * protected:
 *     void onUpdate(double delta_time, const std::vector<sylife::ecs::Entity>& entities) override {
 *         for (auto entity : entities) {
 *             auto& pos = getComponent<Position>(entity);
 *             // Update position logic
 *         }
 *     }
 * };
 * 
 * // Create world and run simulation
 * sylife::ecs::World world;
 * world.registerComponent<Position>();
 * world.registerSystem<MovementSystem>();
 * 
 * // Create entities and add components
 * auto entity = world.createEntity();
 * world.emplaceComponent<Position>(entity, 10.0f, 20.0f);
 * 
 * // Update simulation
 * world.update(0.016); // 60 FPS
 * @endcode
 */

// Core types and concepts
#include "types.h"

// Entity management
#include "entity_manager.h"

// Component storage
#include "component_storage.h"

// System framework
#include "system.h"

// Event system
#include "event_bus.h"

// World coordinator
#include "world.h"

namespace sylife::ecs {

/**
 * @brief ECS framework version information
 */
namespace version {
    constexpr int MAJOR = 1;
    constexpr int MINOR = 0;
    constexpr int PATCH = 0;
    constexpr const char* STRING = "1.0.0";
    constexpr const char* BUILD_DATE = __DATE__;
    constexpr const char* BUILD_TIME = __TIME__;
}

/**
 * @brief ECS framework feature flags
 */
namespace features {
    constexpr bool THREAD_SAFETY = true;
    constexpr bool PERFORMANCE_PROFILING = true;
    constexpr bool EVENT_SYSTEM = true;
    constexpr bool MEMORY_POOLS = true;
    constexpr bool DEPENDENCY_INJECTION = true;
}

/**
 * @brief Utility functions for ECS framework
 */
namespace utils {
    
    /**
     * @brief Create a simple entity with components
     * @param world World to create entity in
     * @param components Components to add to entity
     * @return Created entity
     */
    template<typename... Components>
    Entity makeEntity(World& world, Components&&... components) {
        auto entity = world.createEntity();
        (world.addComponent(entity, std::forward<Components>(components)), ...);
        return entity;
    }
    
    /**
     * @brief Check if an entity has all specified components
     * @tparam Components Component types to check
     * @param world World containing the entity
     * @param entity Entity to check
     * @return true if entity has all components
     */
    template<typename... Components>
    bool hasAllComponents(const World& world, Entity entity) {
        return (world.hasComponent<Components>(entity) && ...);
    }
    
    /**
     * @brief Check if an entity has any of the specified components
     * @tparam Components Component types to check
     * @param world World containing the entity
     * @param entity Entity to check
     * @return true if entity has at least one component
     */
    template<typename... Components>
    bool hasAnyComponent(const World& world, Entity entity) {
        return (world.hasComponent<Components>(entity) || ...);
    }
    
    /**
     * @brief Get component count across all storages
     * @param world World to count components in
     * @return Total number of components
     */
    std::size_t getTotalComponentCount(const World& world);
    
    /**
     * @brief Get memory usage across all ECS subsystems
     * @param world World to analyze
     * @return Total memory usage in bytes
     */
    std::size_t getTotalMemoryUsage(const World& world);
    
    /**
     * @brief Format ECS statistics for debugging
     * @param world World to analyze
     * @return Formatted statistics string
     */
    std::string formatStats(const World& world);
    
} // namespace utils

} // namespace sylife::ecs