#pragma once

#include "types.h"
#include "entity_manager.h"
#include "component_storage.h"
#include "system.h"
#include "event_bus.h"
#include "sylife/di/container.h"
#include <memory>
#include <chrono>

namespace sylife::ecs {

/**
 * @brief ECS World configuration
 */
struct WorldConfig {
    std::size_t max_entities = 100000;
    std::size_t component_reserve_size = 10000;
    bool enable_profiling = true;
    bool enable_events = true;
    double target_fps = 60.0;
    std::size_t max_events_per_frame = 1000;
};

/**
 * @brief Central ECS world that manages all entities, components, systems, and events
 * 
 * The World class is the main entry point for the ECS framework. It coordinates
 * all the major subsystems and provides a unified interface for game logic.
 */
class World {
public:
    explicit World(const WorldConfig& config = WorldConfig{});
    ~World();
    
    // Non-copyable, movable
    World(const World&) = delete;
    World& operator=(const World&) = delete;
    World(World&&) = default;
    World& operator=(World&&) = default;
    
    /**
     * @brief Initialize the world and all systems
     * @param container Dependency injection container
     */
    void initialize(sylife::di::Container& container);
    
    /**
     * @brief Update the world (all systems and events)
     * @param delta_time Time elapsed since last update in seconds
     */
    void update(double delta_time);
    
    /**
     * @brief Shutdown the world and cleanup resources
     */
    void shutdown();
    
    // Entity management
    
    /**
     * @brief Create a new entity
     * @return Newly created entity
     */
    Entity createEntity();
    
    /**
     * @brief Destroy an entity and all its components
     * @param entity Entity to destroy
     */
    void destroyEntity(Entity entity);
    
    /**
     * @brief Check if an entity is valid
     * @param entity Entity to check
     * @return true if entity is valid
     */
    bool isValidEntity(Entity entity) const;
    
    /**
     * @brief Clone an entity with all its components
     * @param source Entity to clone
     * @return Newly created entity with cloned components
     */
    Entity cloneEntity(Entity source);
    
    // Component management
    
    /**
     * @brief Register a component type
     * @tparam T Component type
     */
    template<typename T>
    void registerComponent() {
        component_storage_manager_.registerComponent<T>();
    }
    
    /**
     * @brief Add a component to an entity
     * @tparam T Component type
     * @param entity Entity to add component to
     * @param component Component data
     */
    template<typename T>
    void addComponent(Entity entity, const T& component) {
        validateEntity(entity);
        
        auto& storage = component_storage_manager_.getStorage<T>();
        storage.set(entity, component);
        
        // Update entity signature
        ComponentType type = ComponentTypeRegistry::getComponentType<T>();
        entity_manager_.addComponent(entity, type);
        
        // Publish event
        if (event_bus_) {
            event_bus_->publishDeferred(ComponentAddedEvent(entity, type));
        }
    }
    
    /**
     * @brief Add a component to an entity (move version)
     * @tparam T Component type
     * @param entity Entity to add component to
     * @param component Component data
     */
    template<typename T>
    void addComponent(Entity entity, T&& component) {
        validateEntity(entity);
        
        auto& storage = component_storage_manager_.getStorage<T>();
        storage.set(entity, std::move(component));
        
        // Update entity signature
        ComponentType type = ComponentTypeRegistry::getComponentType<T>();
        entity_manager_.addComponent(entity, type);
        
        // Publish event
        if (event_bus_) {
            event_bus_->publishDeferred(ComponentAddedEvent(entity, type));
        }
    }
    
    /**
     * @brief Emplace a component for an entity
     * @tparam T Component type
     * @tparam Args Constructor argument types
     * @param entity Entity to add component to
     * @param args Constructor arguments
     */
    template<typename T, typename... Args>
    void emplaceComponent(Entity entity, Args&&... args) {
        validateEntity(entity);
        
        auto& storage = component_storage_manager_.getStorage<T>();
        storage.emplace(entity, std::forward<Args>(args)...);
        
        // Update entity signature
        ComponentType type = ComponentTypeRegistry::getComponentType<T>();
        entity_manager_.addComponent(entity, type);
        
        // Publish event
        if (event_bus_) {
            event_bus_->publishDeferred(ComponentAddedEvent(entity, type));
        }
    }
    
    /**
     * @brief Remove a component from an entity
     * @tparam T Component type
     * @param entity Entity to remove component from
     */
    template<typename T>
    void removeComponent(Entity entity) {
        validateEntity(entity);
        
        ComponentType type = ComponentTypeRegistry::getComponentType<T>();
        
        auto& storage = component_storage_manager_.getStorage<T>();
        storage.remove(entity);
        
        // Update entity signature
        entity_manager_.removeComponent(entity, type);
        
        // Publish event
        if (event_bus_) {
            event_bus_->publishDeferred(ComponentRemovedEvent(entity, type));
        }
    }
    
    /**
     * @brief Get a component from an entity
     * @tparam T Component type
     * @param entity Entity to get component from
     * @return Reference to component
     */
    template<typename T>
    T& getComponent(Entity entity) {
        validateEntity(entity);
        return component_storage_manager_.getStorage<T>().get(entity);
    }
    
    /**
     * @brief Get a component from an entity (const version)
     * @tparam T Component type
     * @param entity Entity to get component from
     * @return Const reference to component
     */
    template<typename T>
    const T& getComponent(Entity entity) const {
        validateEntity(entity);
        return component_storage_manager_.getStorage<T>().get(entity);
    }
    
    /**
     * @brief Try to get a component from an entity
     * @tparam T Component type
     * @param entity Entity to get component from
     * @return Pointer to component or nullptr
     */
    template<typename T>
    T* tryGetComponent(Entity entity) {
        if (!isValidEntity(entity)) {
            return nullptr;
        }
        return component_storage_manager_.getStorage<T>().tryGet(entity);
    }
    
    /**
     * @brief Try to get a component from an entity (const version)
     * @tparam T Component type
     * @param entity Entity to get component from
     * @return Pointer to component or nullptr
     */
    template<typename T>
    const T* tryGetComponent(Entity entity) const {
        if (!isValidEntity(entity)) {
            return nullptr;
        }
        return component_storage_manager_.getStorage<T>().tryGet(entity);
    }
    
    /**
     * @brief Check if an entity has a component
     * @tparam T Component type
     * @param entity Entity to check
     * @return true if entity has the component
     */
    template<typename T>
    bool hasComponent(Entity entity) const {
        if (!isValidEntity(entity)) {
            return false;
        }
        ComponentType type = ComponentTypeRegistry::getComponentType<T>();
        return entity_manager_.hasComponent(entity, type);
    }
    
    /**
     * @brief Get all entities with specific components
     * @tparam Components Component types
     * @return Vector of entities with all specified components
     */
    template<typename... Components>
    std::vector<Entity> getEntitiesWithComponents() const {
        return entity_manager_.getEntitiesWithComponents<Components...>();
    }
    
    // System management
    
    /**
     * @brief Register a system
     * @tparam T System type
     * @tparam Args Constructor argument types
     * @param args Constructor arguments
     * @return Reference to registered system
     */
    template<typename T, typename... Args>
    T& registerSystem(Args&&... args) {
        return system_manager_.registerSystem<T>(std::forward<Args>(args)...);
    }
    
    /**
     * @brief Get a system by type and name
     * @tparam T System type
     * @param name System name
     * @return Pointer to system or nullptr
     */
    template<typename T>
    T* getSystem(const std::string& name) {
        return system_manager_.getSystem<T>(name);
    }
    
    // Event management
    
    /**
     * @brief Subscribe to events
     * @tparam T Event type
     * @param handler Event handler function
     * @return Event subscription
     */
    template<typename T>
    EventSubscription subscribeToEvent(EventHandler<T> handler) {
        if (!event_bus_) {
            throw std::runtime_error("Event bus not initialized");
        }
        return event_bus_->subscribe<T>(std::move(handler));
    }
    
    /**
     * @brief Publish an event immediately
     * @tparam T Event type
     * @param event Event to publish
     */
    template<typename T>
    void publishEvent(const T& event) {
        if (event_bus_) {
            event_bus_->publishImmediate(event);
        }
    }
    
    /**
     * @brief Publish an event for deferred processing
     * @tparam T Event type
     * @param event Event to publish
     * @param priority Event priority
     */
    template<typename T>
    void publishEventDeferred(const T& event, EventPriority priority = EventPriority::Normal) {
        if (event_bus_) {
            event_bus_->publishDeferred(event, priority);
        }
    }
    
    // Access to managers (for systems)
    
    /**
     * @brief Get entity manager
     * @return Reference to entity manager
     */
    EntityManager& getEntityManager() { return entity_manager_; }
    
    /**
     * @brief Get entity manager (const version)
     * @return Const reference to entity manager
     */
    const EntityManager& getEntityManager() const { return entity_manager_; }
    
    /**
     * @brief Get component storage manager
     * @return Reference to component storage manager
     */
    ComponentStorageManager& getComponentStorageManager() { return component_storage_manager_; }
    
    /**
     * @brief Get component storage manager (const version)
     * @return Const reference to component storage manager
     */
    const ComponentStorageManager& getComponentStorageManager() const { return component_storage_manager_; }
    
    /**
     * @brief Get system manager
     * @return Reference to system manager
     */
    SystemManager& getSystemManager() { return system_manager_; }
    
    /**
     * @brief Get system manager (const version)
     * @return Const reference to system manager
     */
    const SystemManager& getSystemManager() const { return system_manager_; }
    
    /**
     * @brief Get event bus
     * @return Reference to event bus
     */
    EventBus& getEventBus() { 
        if (!event_bus_) {
            throw std::runtime_error("Event bus not initialized");
        }
        return *event_bus_; 
    }
    
    /**
     * @brief Get event bus (const version)
     * @return Const reference to event bus
     */
    const EventBus& getEventBus() const { 
        if (!event_bus_) {
            throw std::runtime_error("Event bus not initialized");
        }
        return *event_bus_; 
    }
    
    // Statistics and profiling
    
    /**
     * @brief Get world statistics
     * @return Statistics about the world state
     */
    struct WorldStats {
        std::size_t entity_count;
        std::size_t component_count;
        std::size_t system_count;
        std::size_t pending_events;
        double total_update_time_ms;
        std::size_t memory_usage_bytes;
    };
    
    WorldStats getStats() const;
    
    /**
     * @brief Get detailed profiling data
     * @return Combined profiling data for all systems
     */
    ProfileData getProfilingData() const;
    
    /**
     * @brief Reset profiling data
     */
    void resetProfilingData();
    
    /**
     * @brief Get current frame rate
     * @return Frames per second
     */
    double getCurrentFPS() const { return current_fps_; }
    
    /**
     * @brief Get target frame rate
     * @return Target frames per second
     */
    double getTargetFPS() const { return config_.target_fps; }

private:
    WorldConfig config_;
    EntityManager entity_manager_;
    ComponentStorageManager component_storage_manager_;
    SystemManager system_manager_;
    std::unique_ptr<EventBus> event_bus_;
    
    bool initialized_ = false;
    bool running_ = false;
    
    // Performance tracking
    double current_fps_ = 0.0;
    std::chrono::high_resolution_clock::time_point last_frame_time_;
    std::chrono::high_resolution_clock::time_point last_stats_time_;
    std::size_t frame_count_ = 0;
    
    /**
     * @brief Validate that an entity is valid
     * @param entity Entity to validate
     * @throws std::invalid_argument if entity is invalid
     */
    void validateEntity(Entity entity) const;
    
    /**
     * @brief Update frame rate statistics
     */
    void updateFrameRate();
};

} // namespace sylife::ecs

// Implementation of System template methods (must be in header)
namespace sylife::ecs {

template<typename Derived>
EntityManager& System<Derived>::getEntityManager() {
    return world_->getEntityManager();
}

template<typename Derived>
const EntityManager& System<Derived>::getEntityManager() const {
    return world_->getEntityManager();
}

template<typename Derived>
EventBus& System<Derived>::getEventBus() {
    return world_->getEventBus();
}

template<typename Derived>
const EventBus& System<Derived>::getEventBus() const {
    return world_->getEventBus();
}

template<typename Derived>
ComponentStorageManager& System<Derived>::getComponentStorageManager() {
    return world_->getComponentStorageManager();
}

template<typename Derived>
const ComponentStorageManager& System<Derived>::getComponentStorageManager() const {
    return world_->getComponentStorageManager();
}

} // namespace sylife::ecs