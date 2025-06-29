#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <algorithm>

namespace sylife::ecs {

class World; // Forward declaration

/**
 * @brief Base class for all systems
 */
class ISystem {
public:
    virtual ~ISystem() = default;
    
    virtual void init() {}
    virtual void update(float deltaTime) = 0;
    virtual void shutdown() {}
    
    virtual int getPriority() const { return 0; }
    virtual bool isEnabled() const { return m_enabled; }
    virtual void setEnabled(bool enabled) { m_enabled = enabled; }

protected:
    World* m_world = nullptr;
    bool m_enabled = true;
    
    friend class SystemManager;
    void setWorld(World* world) { m_world = world; }
};

/**
 * @brief System with automatic component type registration
 */
template<typename... Components>
class System : public ISystem {
public:
    static constexpr size_t ComponentCount = sizeof...(Components);
    
protected:
    World& getWorld() { 
        assert(m_world != nullptr && "System not registered with world");
        return *m_world; 
    }
    
    const World& getWorld() const { 
        assert(m_world != nullptr && "System not registered with world");
        return *m_world; 
    }
};

/**
 * @brief Manages system lifecycle and execution order
 */
class SystemManager {
private:
    std::vector<std::unique_ptr<ISystem>> m_systems;
    std::unordered_map<std::type_index, ISystem*> m_systemMap;
    bool m_needsSorting = false;
    World* m_world = nullptr;

    void sortSystems() {
        if (!m_needsSorting) return;
        
        std::sort(m_systems.begin(), m_systems.end(), 
                  [](const auto& a, const auto& b) {
                      return a->getPriority() < b->getPriority();
                  });
        m_needsSorting = false;
    }

public:
    explicit SystemManager(World* world) : m_world(world) {}

    template<typename T, typename... Args>
    T* registerSystem(Args&&... args) {
        static_assert(std::is_base_of_v<ISystem, T>, "T must inherit from ISystem");
        
        std::type_index typeName = std::type_index(typeid(T));
        assert(m_systemMap.find(typeName) == m_systemMap.end() && "System already registered");

        auto system = std::make_unique<T>(std::forward<Args>(args)...);
        T* systemPtr = system.get();
        
        system->setWorld(m_world);
        m_systems.push_back(std::move(system));
        m_systemMap[typeName] = systemPtr;
        m_needsSorting = true;

        return systemPtr;
    }

    template<typename T>
    T* getSystem() {
        std::type_index typeName = std::type_index(typeid(T));
        auto it = m_systemMap.find(typeName);
        return it != m_systemMap.end() ? static_cast<T*>(it->second) : nullptr;
    }

    template<typename T>
    void removeSystem() {
        std::type_index typeName = std::type_index(typeid(T));
        auto it = m_systemMap.find(typeName);
        if (it != m_systemMap.end()) {
            // Find and remove from vector
            auto systemIt = std::find_if(m_systems.begin(), m_systems.end(),
                                       [&](const auto& sys) { return sys.get() == it->second; });
            if (systemIt != m_systems.end()) {
                (*systemIt)->shutdown();
                m_systems.erase(systemIt);
            }
            m_systemMap.erase(it);
        }
    }

    void initSystems() {
        for (auto& system : m_systems) {
            if (system->isEnabled()) {
                system->init();
            }
        }
    }

    void updateSystems(float deltaTime) {
        sortSystems();
        
        for (auto& system : m_systems) {
            if (system->isEnabled()) {
                system->update(deltaTime);
            }
        }
    }

    void shutdownSystems() {
        for (auto& system : m_systems) {
            system->shutdown();
        }
    }

    size_t getSystemCount() const {
        return m_systems.size();
    }

    void setSystemEnabled(std::type_index typeIndex, bool enabled) {
        auto it = m_systemMap.find(typeIndex);
        if (it != m_systemMap.end()) {
            it->second->setEnabled(enabled);
        }
    }

    template<typename T>
    void setSystemEnabled(bool enabled) {
        setSystemEnabled(std::type_index(typeid(T)), enabled);
    }
};

} // namespace sylife::ecs