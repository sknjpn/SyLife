#pragma once

#include <cstdint>
#include <functional>

namespace sylife::ecs {

/**
 * @brief Type-safe entity identifier
 * 
 * Uses a 64-bit ID with generation counter to prevent use-after-free bugs.
 * Upper 32 bits: generation, Lower 32 bits: index
 */
class Entity {
public:
    using IndexType = uint32_t;
    using GenerationType = uint32_t;
    using IdType = uint64_t;

private:
    IdType m_id;

public:
    constexpr Entity() noexcept : m_id(0) {}
    
    constexpr Entity(IndexType index, GenerationType generation) noexcept
        : m_id((static_cast<IdType>(generation) << 32) | index) {}

    constexpr explicit Entity(IdType id) noexcept : m_id(id) {}

    [[nodiscard]] constexpr IndexType index() const noexcept {
        return static_cast<IndexType>(m_id & 0xFFFFFFFF);
    }

    [[nodiscard]] constexpr GenerationType generation() const noexcept {
        return static_cast<GenerationType>(m_id >> 32);
    }

    [[nodiscard]] constexpr IdType id() const noexcept {
        return m_id;
    }

    [[nodiscard]] constexpr bool valid() const noexcept {
        return m_id != 0;
    }

    constexpr bool operator==(const Entity& other) const noexcept {
        return m_id == other.m_id;
    }

    constexpr bool operator!=(const Entity& other) const noexcept {
        return m_id != other.m_id;
    }

    constexpr bool operator<(const Entity& other) const noexcept {
        return m_id < other.m_id;
    }
};

static_assert(sizeof(Entity) == sizeof(uint64_t), "Entity should be 64 bits");

} // namespace sylife::ecs

// Hash support for std::unordered_map
template<>
struct std::hash<sylife::ecs::Entity> {
    size_t operator()(const sylife::ecs::Entity& entity) const noexcept {
        return std::hash<sylife::ecs::Entity::IdType>{}(entity.id());
    }
};