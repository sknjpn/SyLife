#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include <type_traits>
#include <string>
#include <mutex>

namespace sylife::di {

/**
 * @brief Exception thrown when dependency injection fails
 */
class DIException : public std::exception {
public:
    explicit DIException(const std::string& message) : m_message(message) {}
    const char* what() const noexcept override { return m_message.c_str(); }

private:
    std::string m_message;
};

/**
 * @brief Dependency Injection Container
 * 
 * Thread-safe container that manages object lifecycles and dependencies.
 * Supports singleton and transient lifetimes.
 */
class Container {
public:
    enum class Lifetime {
        Singleton,  // Single instance shared across all requests
        Transient   // New instance created for each request
    };

private:
    // Type-erased factory function
    using Factory = std::function<std::shared_ptr<void>()>;
    
    struct Registration {
        Factory factory;
        Lifetime lifetime;
        std::shared_ptr<void> singleton_instance;
        std::type_index interface_type;
        std::type_index implementation_type;
        
        // Default constructor
        Registration() 
            : lifetime(Lifetime::Singleton)
            , interface_type(typeid(void))
            , implementation_type(typeid(void)) {}
        
        // Constructor with all parameters
        Registration(Factory f, Lifetime l, std::type_index i_type, std::type_index impl_type)
            : factory(std::move(f))
            , lifetime(l)
            , interface_type(i_type)
            , implementation_type(impl_type) {}
    };

public:
    Container() = default;
    ~Container() = default;

    // Non-copyable, movable
    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;
    Container(Container&&) = default;
    Container& operator=(Container&&) = default;

    /**
     * @brief Register an implementation for an interface
     * @tparam TInterface Interface type
     * @tparam TImplementation Implementation type
     * @tparam Args Constructor argument types
     * @param lifetime Object lifetime management strategy
     * @param args Constructor arguments
     */
    template<typename TInterface, typename TImplementation, typename... Args>
    void registerType(Lifetime lifetime = Lifetime::Singleton, Args&&... args) {
        static_assert(std::is_base_of_v<TInterface, TImplementation> || 
                     std::is_same_v<TInterface, TImplementation>,
                     "TImplementation must inherit from or be the same as TInterface");

        std::lock_guard<std::mutex> lock(m_mutex);
        
        auto factory = [args...](){ 
            return std::static_pointer_cast<void>(
                std::make_shared<TImplementation>(args...)
            ); 
        };

        m_registrations[std::type_index(typeid(TInterface))] = Registration(
            factory,
            lifetime,
            std::type_index(typeid(TInterface)),
            std::type_index(typeid(TImplementation))
        );
    }

    /**
     * @brief Register a factory function for an interface
     * @tparam TInterface Interface type
     * @param factory Factory function that creates instances
     * @param lifetime Object lifetime management strategy
     */
    template<typename TInterface>
    void registerFactory(std::function<std::shared_ptr<TInterface>()> factory,
                        Lifetime lifetime = Lifetime::Singleton) {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        auto type_erased_factory = [factory]() {
            return std::static_pointer_cast<void>(factory());
        };

        m_registrations[std::type_index(typeid(TInterface))] = Registration(
            type_erased_factory,
            lifetime,
            std::type_index(typeid(TInterface)),
            std::type_index(typeid(TInterface))
        );
    }

    /**
     * @brief Register an existing instance as a singleton
     * @tparam TInterface Interface type
     * @param instance Existing instance to register
     */
    template<typename TInterface>
    void registerInstance(std::shared_ptr<TInterface> instance) {
        if (!instance) {
            throw DIException("Cannot register null instance");
        }

        std::lock_guard<std::mutex> lock(m_mutex);
        
        Registration reg(
            nullptr,
            Lifetime::Singleton,
            std::type_index(typeid(TInterface)),
            std::type_index(typeid(*instance))
        );
        reg.singleton_instance = std::static_pointer_cast<void>(instance);
        
        m_registrations[std::type_index(typeid(TInterface))] = std::move(reg);
    }

    /**
     * @brief Resolve an instance of the specified interface
     * @tparam TInterface Interface type to resolve
     * @return Shared pointer to the resolved instance
     * @throws DIException if the type is not registered
     */
    template<typename TInterface>
    std::shared_ptr<TInterface> resolve() {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        auto type_index = std::type_index(typeid(TInterface));
        auto it = m_registrations.find(type_index);
        
        if (it == m_registrations.end()) {
            throw DIException("Type not registered: " + std::string(typeid(TInterface).name()));
        }

        auto& registration = it->second;

        if (registration.lifetime == Lifetime::Singleton) {
            if (!registration.singleton_instance) {
                if (!registration.factory) {
                    throw DIException("No factory available for singleton type: " + 
                                    std::string(typeid(TInterface).name()));
                }
                registration.singleton_instance = registration.factory();
            }
            return std::static_pointer_cast<TInterface>(registration.singleton_instance);
        } else {
            // Transient
            if (!registration.factory) {
                throw DIException("No factory available for transient type: " + 
                                std::string(typeid(TInterface).name()));
            }
            return std::static_pointer_cast<TInterface>(registration.factory());
        }
    }

    /**
     * @brief Check if a type is registered
     * @tparam TInterface Interface type to check
     * @return true if the type is registered
     */
    template<typename TInterface>
    bool isRegistered() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_registrations.find(std::type_index(typeid(TInterface))) != m_registrations.end();
    }

    /**
     * @brief Clear all registrations
     */
    void clear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_registrations.clear();
    }

    /**
     * @brief Get the number of registered types
     */
    size_t getRegistrationCount() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_registrations.size();
    }

private:
    mutable std::mutex m_mutex;
    std::unordered_map<std::type_index, Registration> m_registrations;
};

} // namespace sylife::di