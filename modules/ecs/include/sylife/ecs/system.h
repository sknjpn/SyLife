#pragma once

#include "types.h"
#include "entity_manager.h"
#include "component_storage.h"
#include "event_bus.h"
#include <vector>
#include <memory>
#include <chrono>
#include <string>

namespace sylife::ecs {

// Forward declarations
class World;

/**
 * @brief Base interface for all systems
 */
class ISystem {
public:
    virtual ~ISystem() = default;
    
    /**
     * @brief Initialize the system
     * @param world Reference to the world
     */
    virtual void initialize(World& world) = 0;
    
    /**
     * @brief Update the system
     * @param delta_time Time elapsed since last update in seconds
     */
    virtual void update(double delta_time) = 0;
    
    /**
     * @brief Cleanup system resources
     */
    virtual void cleanup() = 0;
    
    /**
     * @brief Get system priority
     * @return System priority for execution order
     */
    virtual SystemPriority getPriority() const = 0;
    
    /**
     * @brief Get system execution phase
     * @return System execution phase
     */
    virtual SystemPhase getPhase() const = 0;
    
    /**
     * @brief Get system name
     * @return System name for debugging
     */
    virtual const std::string& getName() const = 0;
    
    /**
     * @brief Check if system is enabled
     * @return true if system is enabled
     */
    virtual bool isEnabled() const = 0;
    
    /**
     * @brief Enable or disable the system
     * @param enabled New enabled state
     */
    virtual void setEnabled(bool enabled) = 0;
    
    /**
     * @brief Get performance profiling data
     * @return Profile data for this system
     */
    virtual ProfileData getProfileData() const = 0;
    
    /**
     * @brief Reset performance profiling data
     */
    virtual void resetProfileData() = 0;
};

/**
 * @brief CRTP base class for systems with common functionality
 * 
 * Provides common system functionality including:
 * - Entity filtering based on component requirements
 * - Performance profiling
 * - Event handling integration
 * - Lifecycle management
 */
template<typename Derived>
class System : public ISystem {
public:
    System(std::string name, SystemPriority priority = SystemPriority::Normal,
           SystemPhase phase = SystemPhase::Update)
        : name_(std::move(name)), priority_(priority), phase_(phase), enabled_(true) {}
    
    virtual ~System() = default;
    
    // ISystem interface implementation
    void initialize(World& world) override {
        world_ = &world;
        profile_data_ = ProfileData{};
        
        // Call derived class initialization
        static_cast<Derived*>(this)->onInitialize();
    }
    
    void update(double delta_time) override {
        if (!enabled_ || !world_) {
            return;
        }
        
        auto start_time = std::chrono::high_resolution_clock::now();
        
        // Get entities that match this system's requirements
        auto entities = getMatchingEntities();
        
        // Call derived class update
        static_cast<Derived*>(this)->onUpdate(delta_time, entities);
        
        // Update profiling data
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double, std::milli>(end_time - start_time);
        
        profile_data_.execution_time_ms = duration.count();
        profile_data_.entities_processed = entities.size();
    }
    
    void cleanup() override {
        static_cast<Derived*>(this)->onCleanup();
        world_ = nullptr;
    }
    
    SystemPriority getPriority() const override {
        return priority_;
    }
    
    SystemPhase getPhase() const override {
        return phase_;
    }
    
    const std::string& getName() const override {
        return name_;
    }
    
    bool isEnabled() const override {
        return enabled_;
    }
    
    void setEnabled(bool enabled) override {
        enabled_ = enabled;
    }
    
    ProfileData getProfileData() const override {
        return profile_data_;
    }
    
    void resetProfileData() override {
        profile_data_ = ProfileData{};
    }

protected:
    /**
     * @brief Get the world instance
     * @return Reference to world
     */
    World& getWorld() {
        return *world_;
    }
    
    /**
     * @brief Get the world instance (const version)
     * @return Const reference to world
     */
    const World& getWorld() const {
        return *world_;
    }
    
    /**
     * @brief Subscribe to events
     * @tparam T Event type
     * @param handler Event handler function
     * @return Event subscription
     */
    template<typename T>
    EventSubscription subscribeToEvent(EventHandler<T> handler) {
        auto subscription = getEventBus().subscribe<T>(std::move(handler));
        subscriptions_.push_back(subscription);
        return subscription;
    }
    
    /**
     * @brief Publish an event immediately
     * @tparam T Event type
     * @param event Event to publish
     */
    template<typename T>
    void publishEvent(const T& event) {
        getEventBus().publishImmediate(event);
    }
    
    /**
     * @brief Publish an event for deferred processing
     * @tparam T Event type
     * @param event Event to publish
     * @param priority Event priority
     */
    template<typename T>
    void publishEventDeferred(const T& event, EventPriority priority = EventPriority::Normal) {
        getEventBus().publishDeferred(event, priority);
    }
    
    /**
     * @brief Get component storage for a type
     * @tparam T Component type
     * @return Reference to component storage
     */
    template<typename T>
    ComponentStorage<T>& getComponentStorage() {
        return getComponentStorageManager().template getStorage<T>();
    }
    
    /**
     * @brief Get component storage for a type (const version)
     * @tparam T Component type
     * @return Const reference to component storage
     */
    template<typename T>
    const ComponentStorage<T>& getComponentStorage() const {
        return getComponentStorageManager().template getStorage<T>();
    }
    
    /**
     * @brief Get entity manager
     * @return Reference to entity manager
     */
    EntityManager& getEntityManager();
    
    /**
     * @brief Get entity manager (const version)
     * @return Const reference to entity manager
     */
    const EntityManager& getEntityManager() const;
    
    /**
     * @brief Get event bus
     * @return Reference to event bus
     */
    EventBus& getEventBus();
    
    /**
     * @brief Get event bus (const version)
     * @return Const reference to event bus
     */
    const EventBus& getEventBus() const;
    
    /**
     * @brief Get component storage manager
     * @return Reference to component storage manager
     */
    ComponentStorageManager& getComponentStorageManager();
    
    /**
     * @brief Get component storage manager (const version)
     * @return Const reference to component storage manager
     */
    const ComponentStorageManager& getComponentStorageManager() const;

private:
    World* world_ = nullptr;
    std::string name_;
    SystemPriority priority_;
    SystemPhase phase_;
    bool enabled_;
    ProfileData profile_data_;
    std::vector<EventSubscription> subscriptions_;
    
    /**
     * @brief Get entities that match this system's component requirements
     * @return Vector of matching entities
     */
    virtual std::vector<Entity> getMatchingEntities() {
        // Default implementation returns all entities
        // Derived classes should override this for specific component requirements
        return getEntityManager().getEntitiesWithComponents<>();
    }
    
    // Virtual methods for derived classes to override
    virtual void onInitialize() {}
    virtual void onUpdate(double delta_time, const std::vector<Entity>& entities) = 0;
    virtual void onCleanup() {}
};

/**
 * @brief System that requires specific components
 * 
 * Template specialization that automatically filters entities
 * based on required component types.
 */
template<typename Derived, typename... RequiredComponents>
class ComponentSystem : public System<Derived> {
public:
    using System<Derived>::System;

protected:
    /**
     * @brief Get entities with all required components
     * @return Vector of entities with required components
     */
    std::vector<Entity> getMatchingEntities() override {
        return this->getEntityManager().template getEntitiesWithComponents<RequiredComponents...>();
    }
    
    /**
     * @brief Get component for an entity (with type safety)
     * @tparam T Component type (must be in RequiredComponents)
     * @param entity Entity to get component from
     * @return Reference to component
     */
    template<typename T>
    T& getComponent(Entity entity) {
        static_assert((std::is_same_v<T, RequiredComponents> || ...), 
                     "T must be one of the required components");
        return this->template getComponentStorage<T>().get(entity);
    }
    
    /**
     * @brief Get component for an entity (const version)
     * @tparam T Component type (must be in RequiredComponents)
     * @param entity Entity to get component from
     * @return Const reference to component
     */
    template<typename T>
    const T& getComponent(Entity entity) const {
        static_assert((std::is_same_v<T, RequiredComponents> || ...), 
                     "T must be one of the required components");
        return this->template getComponentStorage<T>().get(entity);
    }
    
    /**
     * @brief Try to get component for an entity
     * @tparam T Component type (must be in RequiredComponents)
     * @param entity Entity to get component from
     * @return Pointer to component or nullptr
     */
    template<typename T>
    T* tryGetComponent(Entity entity) {
        static_assert((std::is_same_v<T, RequiredComponents> || ...), 
                     "T must be one of the required components");
        return this->template getComponentStorage<T>().tryGet(entity);
    }
    
    /**
     * @brief Try to get component for an entity (const version)
     * @tparam T Component type (must be in RequiredComponents)
     * @param entity Entity to get component from
     * @return Pointer to component or nullptr
     */
    template<typename T>
    const T* tryGetComponent(Entity entity) const {
        static_assert((std::is_same_v<T, RequiredComponents> || ...), 
                     "T must be one of the required components");
        return this->template getComponentStorage<T>().tryGet(entity);
    }
};

/**
 * @brief System manager for handling system lifecycle and execution
 */
class SystemManager {
public:
    SystemManager() = default;
    ~SystemManager() = default;
    
    // Non-copyable, movable
    SystemManager(const SystemManager&) = delete;
    SystemManager& operator=(const SystemManager&) = delete;
    SystemManager(SystemManager&&) = default;
    SystemManager& operator=(SystemManager&&) = default;
    
    /**
     * @brief Register a system
     * @tparam T System type
     * @tparam Args Constructor argument types
     * @param args Constructor arguments
     * @return Reference to registered system
     */
    template<typename T, typename... Args>
    T& registerSystem(Args&&... args) {
        auto system = std::make_unique<T>(std::forward<Args>(args)...);
        T& system_ref = *system;
        
        addSystem(std::move(system));
        return system_ref;
    }
    
    /**
     * @brief Add a system instance
     * @param system Unique pointer to system
     */
    void addSystem(std::unique_ptr<ISystem> system);
    
    /**
     * @brief Remove a system by name
     * @param name System name
     * @return true if system was removed
     */
    bool removeSystem(const std::string& name);
    
    /**
     * @brief Get a system by name and type
     * @tparam T System type
     * @param name System name
     * @return Pointer to system or nullptr
     */
    template<typename T>
    T* getSystem(const std::string& name) {
        for (auto& system : systems_) {
            if (system->getName() == name) {
                return dynamic_cast<T*>(system.get());
            }
        }
        return nullptr;
    }
    
    /**
     * @brief Initialize all systems
     * @param world Reference to world
     */
    void initializeAll(World& world);
    
    /**
     * @brief Update systems in a specific phase
     * @param phase System phase to update
     * @param delta_time Time elapsed since last update
     */
    void updatePhase(SystemPhase phase, double delta_time);
    
    /**
     * @brief Update all systems in all phases
     * @param delta_time Time elapsed since last update
     */
    void updateAll(double delta_time);
    
    /**
     * @brief Cleanup all systems
     */
    void cleanupAll();
    
    /**
     * @brief Clear all systems
     */
    void clear();
    
    /**
     * @brief Get number of registered systems
     * @return System count
     */
    std::size_t getSystemCount() const;
    
    /**
     * @brief Get systems in a specific phase
     * @param phase System phase
     * @return Vector of systems in the phase
     */
    std::vector<ISystem*> getSystemsInPhase(SystemPhase phase) const;
    
    /**
     * @brief Get aggregate profiling data for all systems
     * @return Combined profile data
     */
    ProfileData getAggregateProfileData() const;
    
    /**
     * @brief Reset profiling data for all systems
     */
    void resetAllProfileData();

private:
    std::vector<std::unique_ptr<ISystem>> systems_;
    bool initialized_ = false;
    
    /**
     * @brief Sort systems by priority within each phase
     */
    void sortSystems();
};

} // namespace sylife::ecs