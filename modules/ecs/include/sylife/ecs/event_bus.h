#pragma once

#include "types.h"
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include <queue>
#include <mutex>
#include <type_traits>
#include <algorithm>

namespace sylife::ecs {

/**
 * @brief Base class for all events
 */
struct EventBase {
    virtual ~EventBase() = default;
    
    /**
     * @brief Get event type
     * @return Event type identifier
     */
    virtual EventType getType() const = 0;
    
    /**
     * @brief Get event priority
     * @return Event priority
     */
    virtual EventPriority getPriority() const { return EventPriority::Normal; }
    
    /**
     * @brief Get timestamp when event was created
     * @return Timestamp in milliseconds
     */
    virtual std::uint64_t getTimestamp() const = 0;
    
    /**
     * @brief Clone the event
     * @return Unique pointer to cloned event
     */
    virtual std::unique_ptr<EventBase> clone() const = 0;
};

/**
 * @brief CRTP base for typed events
 */
template<typename T>
struct Event : public EventBase {
    Event() : timestamp_(getCurrentTimeMillis()) {}
    
    EventType getType() const override {
        return std::type_index(typeid(T));
    }
    
    std::uint64_t getTimestamp() const override {
        return timestamp_;
    }
    
    std::unique_ptr<EventBase> clone() const override {
        return std::make_unique<T>(static_cast<const T&>(*this));
    }

private:
    std::uint64_t timestamp_;
    
    static std::uint64_t getCurrentTimeMillis() {
        using namespace std::chrono;
        return duration_cast<milliseconds>(
            steady_clock::now().time_since_epoch()
        ).count();
    }
};

/**
 * @brief Event handler function type
 */
template<typename T>
using EventHandler = std::function<void(const T&)>;

/**
 * @brief Type-erased event handler wrapper
 */
class IEventHandler {
public:
    virtual ~IEventHandler() = default;
    virtual void handle(const EventBase& event) = 0;
    virtual EventType getEventType() const = 0;
    virtual std::uint32_t getHandlerId() const = 0;
};

/**
 * @brief Typed event handler wrapper
 */
template<typename T>
class TypedEventHandler : public IEventHandler {
public:
    explicit TypedEventHandler(EventHandler<T> handler, std::uint32_t id)
        : handler_(std::move(handler)), id_(id) {}
    
    void handle(const EventBase& event) override {
        handler_(static_cast<const T&>(event));
    }
    
    EventType getEventType() const override {
        return std::type_index(typeid(T));
    }
    
    std::uint32_t getHandlerId() const override {
        return id_;
    }

private:
    EventHandler<T> handler_;
    std::uint32_t id_;
};

/**
 * @brief Event subscription handle for managing subscriptions
 */
class EventSubscription {
public:
    EventSubscription() = default;
    EventSubscription(EventType type, std::uint32_t handler_id)
        : event_type_(type), handler_id_(handler_id), valid_(true) {}
    
    EventType getEventType() const { return event_type_; }
    std::uint32_t getHandlerId() const { return handler_id_; }
    bool isValid() const { return valid_; }
    
    void invalidate() { valid_ = false; }

private:
    EventType event_type_ = std::type_index(typeid(void));
    std::uint32_t handler_id_ = 0;
    bool valid_ = false;
};

/**
 * @brief Event with priority for ordered processing
 */
struct PrioritizedEvent {
    std::unique_ptr<EventBase> event;
    EventPriority priority;
    std::uint64_t sequence; // For stable sorting
    
    PrioritizedEvent(std::unique_ptr<EventBase> e, EventPriority p, std::uint64_t seq)
        : event(std::move(e)), priority(p), sequence(seq) {}
    
    // Higher priority and earlier sequence number have higher precedence
    bool operator<(const PrioritizedEvent& other) const {
        if (priority != other.priority) {
            return static_cast<std::int32_t>(priority) > 
                   static_cast<std::int32_t>(other.priority);
        }
        return sequence > other.sequence; // Earlier sequence first
    }
};

/**
 * @brief High-performance event bus for inter-system communication
 * 
 * Thread-safe event bus that supports:
 * - Immediate and deferred event processing
 * - Priority-based event ordering
 * - Type-safe event handlers
 * - Subscription management
 */
class EventBus {
public:
    EventBus() = default;
    ~EventBus() = default;
    
    // Non-copyable, movable
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;
    EventBus(EventBus&&) = default;
    EventBus& operator=(EventBus&&) = default;
    
    /**
     * @brief Subscribe to events of a specific type
     * @tparam T Event type
     * @param handler Event handler function
     * @return Subscription handle
     */
    template<typename T>
    EventSubscription subscribe(EventHandler<T> handler) {
        static_assert(std::is_base_of_v<EventBase, T>, "T must inherit from EventBase");
        
        std::lock_guard<std::mutex> lock(mutex_);
        
        EventType event_type = std::type_index(typeid(T));
        std::uint32_t handler_id = next_handler_id_++;
        
        auto typed_handler = std::make_unique<TypedEventHandler<T>>(
            std::move(handler), handler_id
        );
        
        handlers_[event_type].push_back(std::move(typed_handler));
        
        return EventSubscription(event_type, handler_id);
    }
    
    /**
     * @brief Unsubscribe from events
     * @param subscription Subscription handle to cancel
     */
    void unsubscribe(const EventSubscription& subscription) {
        if (!subscription.isValid()) {
            return;
        }
        
        std::lock_guard<std::mutex> lock(mutex_);
        
        auto it = handlers_.find(subscription.getEventType());
        if (it != handlers_.end()) {
            auto& handler_list = it->second;
            handler_list.erase(
                std::remove_if(handler_list.begin(), handler_list.end(),
                    [&subscription](const std::unique_ptr<IEventHandler>& handler) {
                        return handler->getHandlerId() == subscription.getHandlerId();
                    }
                ),
                handler_list.end()
            );
            
            if (handler_list.empty()) {
                handlers_.erase(it);
            }
        }
    }
    
    /**
     * @brief Publish an event immediately
     * @tparam T Event type
     * @param event Event to publish
     */
    template<typename T>
    void publishImmediate(const T& event) {
        static_assert(std::is_base_of_v<EventBase, T>, "T must inherit from EventBase");
        
        std::lock_guard<std::mutex> lock(mutex_);
        publishEventUnsafe(event);
    }
    
    /**
     * @brief Publish an event for deferred processing
     * @tparam T Event type
     * @param event Event to publish
     * @param priority Event priority (default: Normal)
     */
    template<typename T>
    void publishDeferred(const T& event, EventPriority priority = EventPriority::Normal) {
        static_assert(std::is_base_of_v<EventBase, T>, "T must inherit from EventBase");
        
        std::lock_guard<std::mutex> lock(mutex_);
        
        deferred_events_.emplace(
            std::make_unique<T>(event),
            priority,
            sequence_counter_++
        );
    }
    
    /**
     * @brief Process all deferred events
     * @param max_events Maximum number of events to process (0 = all)
     */
    void processEvents(std::size_t max_events = 0) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        std::size_t processed = 0;
        while (!deferred_events_.empty() && (max_events == 0 || processed < max_events)) {
            auto prioritized_event = std::move(const_cast<PrioritizedEvent&>(deferred_events_.top()));
            deferred_events_.pop();
            
            publishEventUnsafe(*prioritized_event.event);
            ++processed;
        }
    }
    
    /**
     * @brief Clear all deferred events
     */
    void clearEvents() {
        std::lock_guard<std::mutex> lock(mutex_);
        while (!deferred_events_.empty()) {
            deferred_events_.pop();
        }
    }
    
    /**
     * @brief Get number of pending deferred events
     * @return Number of deferred events
     */
    std::size_t getPendingEventCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return deferred_events_.size();
    }
    
    /**
     * @brief Get number of registered handlers for an event type
     * @tparam T Event type
     * @return Number of handlers
     */
    template<typename T>
    std::size_t getHandlerCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        
        EventType event_type = std::type_index(typeid(T));
        auto it = handlers_.find(event_type);
        return (it != handlers_.end()) ? it->second.size() : 0;
    }
    
    /**
     * @brief Get total number of registered handlers
     * @return Total handler count
     */
    std::size_t getTotalHandlerCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        
        std::size_t total = 0;
        for (const auto& [type, handler_list] : handlers_) {
            total += handler_list.size();
        }
        return total;
    }
    
    /**
     * @brief Clear all handlers and events
     */
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        handlers_.clear();
        clearEvents();
    }

private:
    mutable std::mutex mutex_;
    std::unordered_map<EventType, std::vector<std::unique_ptr<IEventHandler>>> handlers_;
    std::priority_queue<PrioritizedEvent> deferred_events_;
    std::uint32_t next_handler_id_ = 1;
    std::uint64_t sequence_counter_ = 0;
    
    /**
     * @brief Publish event to handlers (assumes mutex is locked)
     * @param event Event to publish
     */
    void publishEventUnsafe(const EventBase& event) {
        EventType event_type = event.getType();
        auto it = handlers_.find(event_type);
        
        if (it != handlers_.end()) {
            for (const auto& handler : it->second) {
                try {
                    handler->handle(event);
                } catch (const std::exception& e) {
                    // Log error but continue processing other handlers
                    // TODO: Add proper logging
                }
            }
        }
    }
};

// Common event types for ecosystem simulation

/**
 * @brief Event fired when an entity is created
 */
struct EntityCreatedEvent : public Event<EntityCreatedEvent> {
    Entity entity;
    
    explicit EntityCreatedEvent(Entity e) : entity(e) {}
};

/**
 * @brief Event fired when an entity is destroyed
 */
struct EntityDestroyedEvent : public Event<EntityDestroyedEvent> {
    Entity entity;
    
    explicit EntityDestroyedEvent(Entity e) : entity(e) {}
};

/**
 * @brief Event fired when a component is added to an entity
 */
struct ComponentAddedEvent : public Event<ComponentAddedEvent> {
    Entity entity;
    ComponentType component_type;
    
    ComponentAddedEvent(Entity e, ComponentType type) 
        : entity(e), component_type(type) {}
};

/**
 * @brief Event fired when a component is removed from an entity
 */
struct ComponentRemovedEvent : public Event<ComponentRemovedEvent> {
    Entity entity;
    ComponentType component_type;
    
    ComponentRemovedEvent(Entity e, ComponentType type) 
        : entity(e), component_type(type) {}
};

/**
 * @brief Event fired when entities collide
 */
struct CollisionEvent : public Event<CollisionEvent> {
    Entity entity1;
    Entity entity2;
    float impact_force;
    
    CollisionEvent(Entity e1, Entity e2, float force)
        : entity1(e1), entity2(e2), impact_force(force) {}
    
    EventPriority getPriority() const override {
        return EventPriority::High; // Collisions need immediate processing
    }
};

/**
 * @brief Event fired when an entity's health reaches zero
 */
struct EntityDeathEvent : public Event<EntityDeathEvent> {
    Entity entity;
    Entity killer; // Can be null entity
    
    EntityDeathEvent(Entity e, Entity k = Entity{}) 
        : entity(e), killer(k) {}
    
    EventPriority getPriority() const override {
        return EventPriority::High;
    }
};

} // namespace sylife::ecs