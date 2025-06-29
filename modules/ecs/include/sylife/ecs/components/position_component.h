#pragma once

#include "../types.h"
#include <cmath>

namespace sylife::ecs {

/**
 * @brief 2D position component for spatial entities
 */
struct PositionComponent final : public Component<PositionComponent> {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f; // For 3D positioning or layering
    
    /**
     * @brief Default constructor
     */
    PositionComponent() = default;
    
    /**
     * @brief Constructor with 2D position
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate (default 0)
     */
    PositionComponent(float x, float y, float z = 0.0f) : x(x), y(y), z(z) {}
    
    /**
     * @brief Get distance to another position
     * @param other Other position
     * @return Distance between positions
     */
    float distanceTo(const PositionComponent& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
    
    /**
     * @brief Get squared distance to another position (faster than distanceTo)
     * @param other Other position
     * @return Squared distance between positions
     */
    float distanceSquaredTo(const PositionComponent& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return dx * dx + dy * dy + dz * dz;
    }
    
    /**
     * @brief Get 2D distance (ignoring Z)
     * @param other Other position
     * @return 2D distance between positions
     */
    float distance2DTo(const PositionComponent& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
    
    /**
     * @brief Move by offset
     * @param dx X offset
     * @param dy Y offset
     * @param dz Z offset (default 0)
     */
    void move(float dx, float dy, float dz = 0.0f) {
        x += dx;
        y += dy;
        z += dz;
    }
    
    /**
     * @brief Set position
     * @param new_x New X coordinate
     * @param new_y New Y coordinate
     * @param new_z New Z coordinate (default 0)
     */
    void setPosition(float new_x, float new_y, float new_z = 0.0f) {
        x = new_x;
        y = new_y;
        z = new_z;
    }
    
    /**
     * @brief Get position as array
     * @return Array containing [x, y, z]
     */
    std::array<float, 3> toArray() const {
        return {x, y, z};
    }
    
    /**
     * @brief Linear interpolation between positions
     * @param other Target position
     * @param t Interpolation factor (0.0 to 1.0)
     * @return Interpolated position
     */
    PositionComponent lerp(const PositionComponent& other, float t) const {
        return PositionComponent(
            x + (other.x - x) * t,
            y + (other.y - y) * t,
            z + (other.z - z) * t
        );
    }
    
    /**
     * @brief Check if position is approximately equal to another
     * @param other Other position
     * @param epsilon Tolerance for comparison
     * @return true if positions are approximately equal
     */
    bool isApproximatelyEqual(const PositionComponent& other, float epsilon = 0.001f) const {
        return std::abs(x - other.x) < epsilon &&
               std::abs(y - other.y) < epsilon &&
               std::abs(z - other.z) < epsilon;
    }
    
    // Operator overloads for convenience
    PositionComponent operator+(const PositionComponent& other) const {
        return PositionComponent(x + other.x, y + other.y, z + other.z);
    }
    
    PositionComponent operator-(const PositionComponent& other) const {
        return PositionComponent(x - other.x, y - other.y, z - other.z);
    }
    
    PositionComponent operator*(float scalar) const {
        return PositionComponent(x * scalar, y * scalar, z * scalar);
    }
    
    PositionComponent operator/(float scalar) const {
        return PositionComponent(x / scalar, y / scalar, z / scalar);
    }
    
    PositionComponent& operator+=(const PositionComponent& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    
    PositionComponent& operator-=(const PositionComponent& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    
    PositionComponent& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    
    PositionComponent& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
    
    bool operator==(const PositionComponent& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    
    bool operator!=(const PositionComponent& other) const {
        return !(*this == other);
    }
};

} // namespace sylife::ecs