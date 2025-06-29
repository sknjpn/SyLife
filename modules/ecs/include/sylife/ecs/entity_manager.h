#pragma once

#include "types.h"
#include <vector>
#include <queue>
#include <unordered_set>
#include <memory>
#include <mutex>

namespace sylife::ecs {

/**
 * @brief Manages entity lifecycle and component signatures
 * 
 * Thread-safe entity manager that handles entity creation, destruction,
 * and component signature tracking for efficient system queries.
 */
class EntityManager {
public:
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
     * @brief Check if an entity is valid and alive
     * @param entity Entity to check
     * @return true if entity is valid
     */
    bool isValid(Entity entity) const;
    
    /**
     * @brief Get the component signature for an entity
     * @param entity Entity to get signature for
     * @return Component signature
     */
    ComponentSignature getSignature(Entity entity) const;
    
    /**
     * @brief Set the component signature for an entity
     * @param entity Entity to set signature for
     * @param signature New component signature
     */
    void setSignature(Entity entity, ComponentSignature signature);
    
    /**
     * @brief Add a component to an entity's signature
     * @param entity Entity to modify
     * @param component_type Component type to add
     */
    void addComponent(Entity entity, ComponentType component_type);
    
    /**
     * @brief Remove a component from an entity's signature
     * @param entity Entity to modify
     * @param component_type Component type to remove
     */
    void removeComponent(Entity entity, ComponentType component_type);
    
    /**
     * @brief Check if an entity has a specific component
     * @param entity Entity to check
     * @param component_type Component type to check for
     * @return true if entity has the component
     */
    bool hasComponent(Entity entity, ComponentType component_type) const;
    
    /**
     * @brief Get all entities with a specific signature
     * @param signature Component signature to match
     * @return Vector of matching entities
     */
    std::vector<Entity> getEntitiesWithSignature(const ComponentSignature& signature) const;
    
    /**
     * @brief Get all entities that have all specified components
     * @tparam Components Component types to check for
     * @return Vector of matching entities
     */
    template<typename... Components>
    std::vector<Entity> getEntitiesWithComponents() const {
        ComponentSignature signature;
        (signature.set(ComponentTypeRegistry::getComponentIndex<Components>()), ...);
        return getEntitiesWithSignature(signature);
    }
    
    /**
     * @brief Get total number of alive entities
     * @return Number of alive entities
     */
    std::size_t getEntityCount() const;
    
    /**
     * @brief Get maximum number of entities that can be created
     * @return Maximum entity count
     */
    std::size_t getMaxEntityCount() const;
    
    /**
     * @brief Reserve space for entities to reduce allocations
     * @param count Number of entities to reserve space for
     */
    void reserve(std::size_t count);
    
    /**
     * @brief Clear all entities and reset the manager
     */
    void clear();
    
    /**
     * @brief Get memory usage statistics
     * @return Memory usage in bytes
     */
    std::size_t getMemoryUsage() const;

private:
    static constexpr std::size_t MAX_ENTITIES = 100000;
    
    struct EntityInfo {
        bool alive = false;
        Entity::VersionType version = 1;
        ComponentSignature signature;
    };
    
    mutable std::mutex mutex_;
    std::vector<EntityInfo> entities_;
    std::queue<Entity::IdType> available_ids_;
    Entity::IdType next_id_ = 1; // Start from 1, 0 is NULL_ID
    std::size_t alive_count_ = 0;
    
    /**
     * @brief Get entity info by ID (thread-safe)
     * @param id Entity ID
     * @return Reference to entity info
     */
    const EntityInfo& getEntityInfo(Entity::IdType id) const;
    
    /**
     * @brief Get mutable entity info by ID (thread-safe)
     * @param id Entity ID
     * @return Reference to entity info
     */
    EntityInfo& getEntityInfo(Entity::IdType id);
    
    /**
     * @brief Validate entity ID
     * @param id Entity ID to validate
     * @return true if ID is valid
     */
    bool isValidId(Entity::IdType id) const;
};

} // namespace sylife::ecs