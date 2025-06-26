#pragma once

#include "common.h"

namespace sylife::core {

/**
 * @brief Base class for all SyLife objects that can be serialized
 */
class Object {
public:
    Object() = default;
    virtual ~Object() = default;

    // Delete copy constructor and assignment operator
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;

    // Allow move construction and assignment
    Object(Object&&) = default;
    Object& operator=(Object&&) = default;

    /**
     * @brief Serialize object to JSON
     * @param json JSON object to save to
     */
    virtual void save(s3d::JSON& json) const = 0;

    /**
     * @brief Deserialize object from JSON
     * @param json JSON object to load from
     */
    virtual void load(const s3d::JSON& json) = 0;

    /**
     * @brief Get the type name of this object
     * @return Type name as string
     */
    virtual s3d::String getTypeName() const = 0;

    /**
     * @brief Get unique identifier for this object
     * @return Unique ID
     */
    UniqueId getId() const { return id_; }

    /**
     * @brief Check if this object is valid
     * @return true if valid, false otherwise
     */
    virtual bool isValid() const { return true; }

    /**
     * @brief Clone this object
     * @return Unique pointer to cloned object
     */
    virtual std::unique_ptr<Object> clone() const = 0;

protected:
    /**
     * @brief Generate a new unique ID
     * @return New unique ID
     */
    static UniqueId generateId();

private:
    UniqueId id_ = generateId();
};

/**
 * @brief RAII guard for object state management
 */
template<typename T>
class ObjectGuard {
public:
    explicit ObjectGuard(T* object) : object_(object) {
        if (object_) {
            object_->onEnterScope();
        }
    }

    ~ObjectGuard() {
        if (object_) {
            object_->onExitScope();
        }
    }

    ObjectGuard(const ObjectGuard&) = delete;
    ObjectGuard& operator=(const ObjectGuard&) = delete;

    ObjectGuard(ObjectGuard&& other) noexcept : object_(other.object_) {
        other.object_ = nullptr;
    }

    ObjectGuard& operator=(ObjectGuard&& other) noexcept {
        if (this != &other) {
            if (object_) {
                object_->onExitScope();
            }
            object_ = other.object_;
            other.object_ = nullptr;
        }
        return *this;
    }

    T* get() const { return object_; }
    T* operator->() const { return object_; }
    T& operator*() const { return *object_; }

private:
    T* object_;
};

} // namespace sylife::core