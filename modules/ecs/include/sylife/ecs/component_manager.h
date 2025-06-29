#pragma once

#include "entity.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <type_traits>
#include <cassert>

namespace sylife::ecs {

/**
 * @brief Base class for component storage
 */
class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void entityDestroyed(Entity entity) = 0;
    virtual size_t size() const = 0;
};

/**
 * @brief Type-specific component storage with cache-friendly dense array
 */
template<typename T>
class ComponentArray : public IComponentArray {
    static_assert(std::is_move_constructible_v<T>, "Component must be move constructible");
    static_assert(std::is_move_assignable_v<T>, "Component must be move assignable");

private:
    std::vector<T> m_components;
    std::unordered_map<Entity, size_t> m_entityToIndex;
    std::unordered_map<size_t, Entity> m_indexToEntity;

public:
    void insertData(Entity entity, T component) {
        assert(m_entityToIndex.find(entity) == m_entityToIndex.end() && "Component added to same entity more than once");

        size_t newIndex = m_components.size();
        m_entityToIndex[entity] = newIndex;
        m_indexToEntity[newIndex] = entity;
        m_components.emplace_back(std::move(component));
    }

    void removeData(Entity entity) {
        assert(m_entityToIndex.find(entity) != m_entityToIndex.end() && "Removing non-existent component");

        size_t indexOfRemovedEntity = m_entityToIndex[entity];
        size_t indexOfLastElement = m_components.size() - 1;
        
        // Move last element to removed element's place
        if (indexOfRemovedEntity != indexOfLastElement) {
            m_components[indexOfRemovedEntity] = std::move(m_components[indexOfLastElement]);

            Entity entityOfLastElement = m_indexToEntity[indexOfLastElement];
            m_entityToIndex[entityOfLastElement] = indexOfRemovedEntity;
            m_indexToEntity[indexOfRemovedEntity] = entityOfLastElement;
        }

        m_entityToIndex.erase(entity);
        m_indexToEntity.erase(indexOfLastElement);
        m_components.pop_back();
    }

    T& getData(Entity entity) {
        assert(m_entityToIndex.find(entity) != m_entityToIndex.end() && "Retrieving non-existent component");
        return m_components[m_entityToIndex[entity]];
    }

    const T& getData(Entity entity) const {
        assert(m_entityToIndex.find(entity) != m_entityToIndex.end() && "Retrieving non-existent component");
        return m_components[m_entityToIndex.at(entity)];
    }

    bool hasData(Entity entity) const {
        return m_entityToIndex.find(entity) != m_entityToIndex.end();
    }

    void entityDestroyed(Entity entity) override {
        if (m_entityToIndex.find(entity) != m_entityToIndex.end()) {
            removeData(entity);
        }
    }

    size_t size() const override {
        return m_components.size();
    }

    // Iterator support for systems
    auto begin() { return m_components.begin(); }
    auto end() { return m_components.end(); }
    auto begin() const { return m_components.begin(); }
    auto end() const { return m_components.end(); }

    // Access to entity mapping for systems that need entity information
    const std::unordered_map<size_t, Entity>& getIndexToEntity() const {
        return m_indexToEntity;
    }
};

/**
 * @brief Manages all component types
 */
class ComponentManager {
private:
    std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> m_componentArrays;

    template<typename T>
    ComponentArray<T>* getComponentArray() {
        std::type_index typeName = std::type_index(typeid(T));
        
        auto it = m_componentArrays.find(typeName);
        if (it == m_componentArrays.end()) {
            m_componentArrays[typeName] = std::make_unique<ComponentArray<T>>();
        }
        
        return static_cast<ComponentArray<T>*>(m_componentArrays[typeName].get());
    }

public:
    template<typename T>
    void addComponent(Entity entity, T component) {
        getComponentArray<T>()->insertData(entity, std::move(component));
    }

    template<typename T>
    void removeComponent(Entity entity) {
        getComponentArray<T>()->removeData(entity);
    }

    template<typename T>
    T& getComponent(Entity entity) {
        return getComponentArray<T>()->getData(entity);
    }

    template<typename T>
    const T& getComponent(Entity entity) const {
        return const_cast<ComponentManager*>(this)->getComponentArray<T>()->getData(entity);
    }

    template<typename T>
    bool hasComponent(Entity entity) const {
        return const_cast<ComponentManager*>(this)->getComponentArray<T>()->hasData(entity);
    }

    void entityDestroyed(Entity entity) {
        for (auto& pair : m_componentArrays) {
            pair.second->entityDestroyed(entity);
        }
    }

    template<typename T>
    ComponentArray<T>* getComponentArray() const {
        return const_cast<ComponentManager*>(this)->getComponentArray<T>();
    }

    size_t getComponentTypeCount() const {
        return m_componentArrays.size();
    }
};

} // namespace sylife::ecs