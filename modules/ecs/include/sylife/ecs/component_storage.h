#pragma once

#include "types.h"
#include "sylife/utils/memory_pool.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <type_traits>
#include <algorithm>
#include <mutex>

namespace sylife::ecs {

/**
 * @brief Base interface for component storage
 */
class IComponentStorage {
public:
    virtual ~IComponentStorage() = default;
    
    /**
     * @brief Remove component for an entity
     * @param entity Entity to remove component from
     */
    virtual void remove(Entity entity) = 0;
    
    /**
     * @brief Check if entity has a component
     * @param entity Entity to check
     * @return true if entity has the component
     */
    virtual bool has(Entity entity) const = 0;
    
    /**
     * @brief Clear all components
     */
    virtual void clear() = 0;
    
    /**
     * @brief Get number of components stored
     * @return Component count
     */
    virtual std::size_t size() const = 0;
    
    /**
     * @brief Get memory usage in bytes
     * @return Memory usage
     */
    virtual std::size_t getMemoryUsage() const = 0;
    
    /**
     * @brief Get component type
     * @return Component type
     */
    virtual ComponentType getComponentType() const = 0;
    
    /**
     * @brief Clone component for an entity
     * @param from_entity Source entity
     * @param to_entity Destination entity
     */
    virtual void clone(Entity from_entity, Entity to_entity) = 0;
};

/**
 * @brief Cache-friendly component storage implementation
 * 
 * Uses structure-of-arrays (SoA) layout for optimal cache performance.
 * Components are stored in contiguous memory with parallel arrays for
 * entity mapping and component data.
 */
template<typename T>
class ComponentStorage : public IComponentStorage {
    static_assert(std::is_base_of_v<ComponentBase, T>, "T must inherit from ComponentBase");
    static_assert(std::is_final_v<T>, "Component types must be final");

public:
    using ComponentType = T;
    using Iterator = typename std::vector<T>::iterator;
    using ConstIterator = typename std::vector<T>::const_iterator;
    
    ComponentStorage() = default;
    ~ComponentStorage() = default;
    
    // Non-copyable, movable
    ComponentStorage(const ComponentStorage&) = delete;
    ComponentStorage& operator=(const ComponentStorage&) = delete;
    ComponentStorage(ComponentStorage&&) = default;
    ComponentStorage& operator=(ComponentStorage&&) = default;
    
    /**
     * @brief Add or update component for an entity
     * @param entity Entity to add component to
     * @param component Component data
     */
    void set(Entity entity, const T& component) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        auto it = entity_to_index_.find(entity);
        if (it != entity_to_index_.end()) {
            // Update existing component
            components_[it->second] = component;
        } else {
            // Add new component
            std::size_t index = components_.size();
            components_.push_back(component);
            entities_.push_back(entity);
            entity_to_index_[entity] = index;
        }
    }
    
    /**
     * @brief Add or update component for an entity (move version)
     * @param entity Entity to add component to
     * @param component Component data
     */
    void set(Entity entity, T&& component) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        auto it = entity_to_index_.find(entity);
        if (it != entity_to_index_.end()) {
            // Update existing component
            components_[it->second] = std::move(component);
        } else {
            // Add new component
            std::size_t index = components_.size();
            components_.emplace_back(std::move(component));
            entities_.push_back(entity);
            entity_to_index_[entity] = index;
        }
    }
    
    /**
     * @brief Emplace component for an entity
     * @tparam Args Constructor argument types
     * @param entity Entity to add component to
     * @param args Constructor arguments
     */
    template<typename... Args>
    void emplace(Entity entity, Args&&... args) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        auto it = entity_to_index_.find(entity);
        if (it != entity_to_index_.end()) {
            // Update existing component
            components_[it->second] = T(std::forward<Args>(args)...);
        } else {
            // Add new component
            std::size_t index = components_.size();
            components_.emplace_back(std::forward<Args>(args)...);
            entities_.push_back(entity);
            entity_to_index_[entity] = index;
        }
    }
    
    /**
     * @brief Get component for an entity
     * @param entity Entity to get component from
     * @return Reference to component
     * @throws std::out_of_range if component doesn't exist
     */
    T& get(Entity entity) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        auto it = entity_to_index_.find(entity);
        if (it == entity_to_index_.end()) {
            throw std::out_of_range("Component not found for entity");
        }
        
        return components_[it->second];
    }
    
    /**
     * @brief Get component for an entity (const version)
     * @param entity Entity to get component from
     * @return Const reference to component
     * @throws std::out_of_range if component doesn't exist
     */
    const T& get(Entity entity) const {
        std::lock_guard<std::mutex> lock(mutex_);
        
        auto it = entity_to_index_.find(entity);
        if (it == entity_to_index_.end()) {
            throw std::out_of_range("Component not found for entity");
        }
        
        return components_[it->second];
    }
    
    /**
     * @brief Try to get component for an entity
     * @param entity Entity to get component from
     * @return Pointer to component or nullptr if not found
     */
    T* tryGet(Entity entity) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        auto it = entity_to_index_.find(entity);
        if (it == entity_to_index_.end()) {
            return nullptr;
        }
        
        return &components_[it->second];
    }
    
    /**
     * @brief Try to get component for an entity (const version)
     * @param entity Entity to get component from
     * @return Pointer to component or nullptr if not found
     */
    const T* tryGet(Entity entity) const {
        std::lock_guard<std::mutex> lock(mutex_);
        
        auto it = entity_to_index_.find(entity);
        if (it == entity_to_index_.end()) {
            return nullptr;
        }
        
        return &components_[it->second];
    }
    
    /**
     * @brief Remove component for an entity
     * @param entity Entity to remove component from
     */
    void remove(Entity entity) override {
        std::lock_guard<std::mutex> lock(mutex_);
        removeUnsafe(entity);
    }
    
    /**
     * @brief Check if entity has a component
     * @param entity Entity to check
     * @return true if entity has the component
     */
    bool has(Entity entity) const override {
        std::lock_guard<std::mutex> lock(mutex_);
        return entity_to_index_.find(entity) != entity_to_index_.end();
    }
    
    /**
     * @brief Clear all components
     */
    void clear() override {
        std::lock_guard<std::mutex> lock(mutex_);
        components_.clear();
        entities_.clear();
        entity_to_index_.clear();
    }
    
    /**
     * @brief Get number of components stored
     * @return Component count
     */
    std::size_t size() const override {
        std::lock_guard<std::mutex> lock(mutex_);
        return components_.size();
    }
    
    /**
     * @brief Get memory usage in bytes
     * @return Memory usage
     */
    std::size_t getMemoryUsage() const override {
        std::lock_guard<std::mutex> lock(mutex_);
        return components_.capacity() * sizeof(T) +
               entities_.capacity() * sizeof(Entity) +
               entity_to_index_.size() * (sizeof(Entity) + sizeof(std::size_t));
    }
    
    /**
     * @brief Get component type
     * @return Component type
     */
    sylife::ecs::ComponentType getComponentType() const override {
        return ComponentTypeRegistry::getComponentType<T>();
    }
    
    /**
     * @brief Clone component for an entity
     * @param from_entity Source entity
     * @param to_entity Destination entity
     */
    void clone(Entity from_entity, Entity to_entity) override {
        std::lock_guard<std::mutex> lock(mutex_);
        
        auto it = entity_to_index_.find(from_entity);
        if (it != entity_to_index_.end()) {
            set(to_entity, components_[it->second]);
        }
    }
    
    /**
     * @brief Reserve space for components
     * @param count Number of components to reserve space for
     */
    void reserve(std::size_t count) {
        std::lock_guard<std::mutex> lock(mutex_);
        components_.reserve(count);
        entities_.reserve(count);
        entity_to_index_.reserve(count);
    }
    
    /**
     * @brief Get all components with their entities
     * @return Vector of entity-component pairs
     */
    std::vector<std::pair<Entity, T*>> getAllComponents() {
        std::lock_guard<std::mutex> lock(mutex_);
        
        std::vector<std::pair<Entity, T*>> result;
        result.reserve(components_.size());
        
        for (std::size_t i = 0; i < components_.size(); ++i) {
            result.emplace_back(entities_[i], &components_[i]);
        }
        
        return result;
    }
    
    /**
     * @brief Get iterator to beginning of components
     * @return Iterator to first component
     */
    Iterator begin() {
        std::lock_guard<std::mutex> lock(mutex_);
        return components_.begin();
    }
    
    /**
     * @brief Get iterator to end of components
     * @return Iterator past last component
     */
    Iterator end() {
        std::lock_guard<std::mutex> lock(mutex_);
        return components_.end();
    }
    
    /**
     * @brief Get const iterator to beginning of components
     * @return Const iterator to first component
     */
    ConstIterator begin() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return components_.begin();
    }
    
    /**
     * @brief Get const iterator to end of components
     * @return Const iterator past last component
     */
    ConstIterator end() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return components_.end();
    }
    
    /**
     * @brief Get const iterator to beginning of components
     * @return Const iterator to first component
     */
    ConstIterator cbegin() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return components_.cbegin();
    }
    
    /**
     * @brief Get const iterator to end of components
     * @return Const iterator past last component
     */
    ConstIterator cend() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return components_.cend();
    }

private:
    mutable std::mutex mutex_;
    
    // Structure of Arrays for cache-friendly access
    std::vector<T> components_;           // Component data
    std::vector<Entity> entities_;        // Entity IDs in same order as components
    std::unordered_map<Entity, std::size_t> entity_to_index_; // Fast entity->index lookup
    
    /**
     * @brief Remove component without locking (assumes mutex is already locked)
     * @param entity Entity to remove component from
     */
    void removeUnsafe(Entity entity) {
        auto it = entity_to_index_.find(entity);
        if (it == entity_to_index_.end()) {
            return; // Component doesn't exist
        }
        
        std::size_t index = it->second;
        std::size_t last_index = components_.size() - 1;
        
        if (index != last_index) {
            // Swap with last element to maintain contiguous array
            components_[index] = std::move(components_[last_index]);
            entities_[index] = entities_[last_index];
            
            // Update index mapping for moved element
            entity_to_index_[entities_[index]] = index;
        }
        
        // Remove last element
        components_.pop_back();
        entities_.pop_back();
        entity_to_index_.erase(entity);
    }
};

/**
 * @brief Component storage manager
 * 
 * Manages multiple component storages and provides unified interface
 * for component operations across different types.
 */
class ComponentStorageManager {
public:
    ComponentStorageManager() = default;
    ~ComponentStorageManager() = default;
    
    // Non-copyable, movable
    ComponentStorageManager(const ComponentStorageManager&) = delete;
    ComponentStorageManager& operator=(const ComponentStorageManager&) = delete;
    ComponentStorageManager(ComponentStorageManager&&) = default;
    ComponentStorageManager& operator=(ComponentStorageManager&&) = default;
    
    /**
     * @brief Register component storage for a type
     * @tparam T Component type
     */
    template<typename T>
    void registerComponent() {
        ComponentType type = ComponentTypeRegistry::getComponentType<T>();
        
        std::lock_guard<std::mutex> lock(mutex_);
        if (storages_.find(type) == storages_.end()) {
            storages_[type] = std::make_unique<ComponentStorage<T>>();
        }
    }
    
    /**
     * @brief Get component storage for a type
     * @tparam T Component type
     * @return Reference to component storage
     */
    template<typename T>
    ComponentStorage<T>& getStorage() {
        ComponentType type = ComponentTypeRegistry::getComponentType<T>();
        
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = storages_.find(type);
        if (it == storages_.end()) {
            throw std::runtime_error("Component type not registered");
        }
        
        return static_cast<ComponentStorage<T>&>(*it->second);
    }
    
    /**
     * @brief Get component storage for a type (const version)
     * @tparam T Component type
     * @return Const reference to component storage
     */
    template<typename T>
    const ComponentStorage<T>& getStorage() const {
        ComponentType type = ComponentTypeRegistry::getComponentType<T>();
        
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = storages_.find(type);
        if (it == storages_.end()) {
            throw std::runtime_error("Component type not registered");
        }
        
        return static_cast<const ComponentStorage<T>&>(*it->second);
    }
    
    /**
     * @brief Remove all components for an entity
     * @param entity Entity to remove components from
     */
    void removeAllComponents(Entity entity) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& [type, storage] : storages_) {
            storage->remove(entity);
        }
    }
    
    /**
     * @brief Clear all component storages
     */
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& [type, storage] : storages_) {
            storage->clear();
        }
    }
    
    /**
     * @brief Get total memory usage across all storages
     * @return Total memory usage in bytes
     */
    std::size_t getTotalMemoryUsage() const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::size_t total = 0;
        for (const auto& [type, storage] : storages_) {
            total += storage->getMemoryUsage();
        }
        return total;
    }
    
    /**
     * @brief Get number of registered component types
     * @return Number of component types
     */
    std::size_t getRegisteredComponentCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return storages_.size();
    }

private:
    mutable std::mutex mutex_;
    std::unordered_map<ComponentType, std::unique_ptr<IComponentStorage>> storages_;
};

} // namespace sylife::ecs